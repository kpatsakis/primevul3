static void loop_reread_partitions(struct loop_device *lo,
				   struct block_device *bdev)
{
	int rc;

	/*
	 * bd_mutex has been held already in release path, so don't
	 * acquire it if this function is called in such case.
	 *
	 * If the reread partition isn't from release path, lo_refcnt
	 * must be at least one and it can only become zero when the
	 * current holder is released.
	 */
	if (!atomic_read(&lo->lo_refcnt))
		rc = __blkdev_reread_part(bdev);
	else
		rc = blkdev_reread_part(bdev);
	if (rc)
		pr_warn("%s: partition scan of loop%d (%s) failed (rc=%d)\n",
			__func__, lo->lo_number, lo->lo_file_name, rc);
}
