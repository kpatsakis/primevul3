static int lo_discard(struct loop_device *lo, struct request *rq, loff_t pos)
{
	/*
	 * We use punch hole to reclaim the free space used by the
	 * image a.k.a. discard. However we do not support discard if
	 * encryption is enabled, because it may give an attacker
	 * useful information.
	 */
	struct file *file = lo->lo_backing_file;
	int mode = FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE;
	int ret;

	if ((!file->f_op->fallocate) || lo->lo_encrypt_key_size) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = file->f_op->fallocate(file, mode, pos, blk_rq_bytes(rq));
	if (unlikely(ret && ret != -EINVAL && ret != -EOPNOTSUPP))
		ret = -EIO;
 out:
	return ret;
}
