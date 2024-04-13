figure_loop_size(struct loop_device *lo, loff_t offset, loff_t sizelimit)
{
	loff_t size = get_size(offset, sizelimit, lo->lo_backing_file);
	sector_t x = (sector_t)size;
	struct block_device *bdev = lo->lo_device;

	if (unlikely((loff_t)x != size))
		return -EFBIG;
	if (lo->lo_offset != offset)
		lo->lo_offset = offset;
	if (lo->lo_sizelimit != sizelimit)
		lo->lo_sizelimit = sizelimit;
	set_capacity(lo->lo_disk, x);
	bd_set_size(bdev, (loff_t)get_capacity(bdev->bd_disk) << 9);
	/* let user-space know about the new size */
	kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, KOBJ_CHANGE);
	return 0;
}
