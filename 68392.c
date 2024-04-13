static int lo_req_flush(struct loop_device *lo, struct request *rq)
{
	struct file *file = lo->lo_backing_file;
	int ret = vfs_fsync(file, 0);
	if (unlikely(ret && ret != -EINVAL))
		ret = -EIO;

	return ret;
}
