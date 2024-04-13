static int lo_open(struct block_device *bdev, fmode_t mode)
{
	struct loop_device *lo;
	int err = 0;

	mutex_lock(&loop_index_mutex);
	lo = bdev->bd_disk->private_data;
	if (!lo) {
		err = -ENXIO;
		goto out;
	}

	atomic_inc(&lo->lo_refcnt);
out:
	mutex_unlock(&loop_index_mutex);
 	return err;
 }
