static int loop_clr_fd(struct loop_device *lo)
{
	struct file *filp = lo->lo_backing_file;
	gfp_t gfp = lo->old_gfp_mask;
	struct block_device *bdev = lo->lo_device;

	if (lo->lo_state != Lo_bound)
		return -ENXIO;

	/*
	 * If we've explicitly asked to tear down the loop device,
	 * and it has an elevated reference count, set it for auto-teardown when
	 * the last reference goes away. This stops $!~#$@ udev from
	 * preventing teardown because it decided that it needs to run blkid on
	 * the loopback device whenever they appear. xfstests is notorious for
	 * failing tests because blkid via udev races with a losetup
	 * <dev>/do something like mkfs/losetup -d <dev> causing the losetup -d
	 * command to fail with EBUSY.
	 */
	if (atomic_read(&lo->lo_refcnt) > 1) {
		lo->lo_flags |= LO_FLAGS_AUTOCLEAR;
		mutex_unlock(&lo->lo_ctl_mutex);
		return 0;
	}

	if (filp == NULL)
		return -EINVAL;

	/* freeze request queue during the transition */
	blk_mq_freeze_queue(lo->lo_queue);

	spin_lock_irq(&lo->lo_lock);
	lo->lo_state = Lo_rundown;
	lo->lo_backing_file = NULL;
	spin_unlock_irq(&lo->lo_lock);

	loop_release_xfer(lo);
	lo->transfer = NULL;
	lo->ioctl = NULL;
	lo->lo_device = NULL;
	lo->lo_encryption = NULL;
	lo->lo_offset = 0;
	lo->lo_sizelimit = 0;
	lo->lo_encrypt_key_size = 0;
	memset(lo->lo_encrypt_key, 0, LO_KEY_SIZE);
	memset(lo->lo_crypt_name, 0, LO_NAME_SIZE);
	memset(lo->lo_file_name, 0, LO_NAME_SIZE);
	blk_queue_logical_block_size(lo->lo_queue, 512);
	blk_queue_physical_block_size(lo->lo_queue, 512);
	blk_queue_io_min(lo->lo_queue, 512);
	if (bdev) {
		bdput(bdev);
		invalidate_bdev(bdev);
	}
	set_capacity(lo->lo_disk, 0);
	loop_sysfs_exit(lo);
	if (bdev) {
		bd_set_size(bdev, 0);
		/* let user-space know about this change */
		kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, KOBJ_CHANGE);
	}
	mapping_set_gfp_mask(filp->f_mapping, gfp);
	lo->lo_state = Lo_unbound;
	/* This is safe: open() is still holding a reference. */
	module_put(THIS_MODULE);
	blk_mq_unfreeze_queue(lo->lo_queue);

	if (lo->lo_flags & LO_FLAGS_PARTSCAN && bdev)
		loop_reread_partitions(lo, bdev);
	lo->lo_flags = 0;
	if (!part_shift)
		lo->lo_disk->flags |= GENHD_FL_NO_PART_SCAN;
	loop_unprepare_queue(lo);
	mutex_unlock(&lo->lo_ctl_mutex);
	/*
	 * Need not hold lo_ctl_mutex to fput backing file.
	 * Calling fput holding lo_ctl_mutex triggers a circular
	 * lock dependency possibility warning as fput can take
	 * bd_mutex which is usually taken before lo_ctl_mutex.
	 */
	fput(filp);
	return 0;
}
