static ssize_t aio_run_iocb(struct kiocb *req, unsigned opcode,
			    char __user *buf, size_t len, bool compat)
{
	struct file *file = req->ki_filp;
	ssize_t ret;
	int rw;
	fmode_t mode;
	rw_iter_op *iter_op;
	struct iovec inline_vecs[UIO_FASTIOV], *iovec = inline_vecs;
	struct iov_iter iter;

	switch (opcode) {
	case IOCB_CMD_PREAD:
	case IOCB_CMD_PREADV:
		mode	= FMODE_READ;
		rw	= READ;
		iter_op	= file->f_op->read_iter;
		goto rw_common;

	case IOCB_CMD_PWRITE:
	case IOCB_CMD_PWRITEV:
		mode	= FMODE_WRITE;
		rw	= WRITE;
		iter_op	= file->f_op->write_iter;
		goto rw_common;
rw_common:
		if (unlikely(!(file->f_mode & mode)))
			return -EBADF;

		if (!iter_op)
			return -EINVAL;

		if (opcode == IOCB_CMD_PREADV || opcode == IOCB_CMD_PWRITEV)
			ret = aio_setup_vectored_rw(rw, buf, len,
						&iovec, compat, &iter);
		else {
			ret = import_single_range(rw, buf, len, iovec, &iter);
			iovec = NULL;
		}
		if (!ret)
			ret = rw_verify_area(rw, file, &req->ki_pos,
					     iov_iter_count(&iter));
		if (ret < 0) {
			kfree(iovec);
			return ret;
		}

		if (rw == WRITE)
			file_start_write(file);

		ret = iter_op(req, &iter);

		if (rw == WRITE)
			file_end_write(file);
		kfree(iovec);
		break;

	case IOCB_CMD_FDSYNC:
		if (!file->f_op->aio_fsync)
			return -EINVAL;

		ret = file->f_op->aio_fsync(req, 1);
		break;

	case IOCB_CMD_FSYNC:
		if (!file->f_op->aio_fsync)
			return -EINVAL;

		ret = file->f_op->aio_fsync(req, 0);
		break;

	default:
		pr_debug("EINVAL: no operation provided\n");
		return -EINVAL;
	}

	if (ret != -EIOCBQUEUED) {
		/*
		 * There's no easy way to restart the syscall since other AIO's
		 * may be already running. Just fail this IO with EINTR.
		 */
		if (unlikely(ret == -ERESTARTSYS || ret == -ERESTARTNOINTR ||
			     ret == -ERESTARTNOHAND ||
			     ret == -ERESTART_RESTARTBLOCK))
			ret = -EINTR;
		aio_complete(req, ret, 0);
	}

	return 0;
}
