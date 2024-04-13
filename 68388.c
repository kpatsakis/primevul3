static int lo_ioctl(struct block_device *bdev, fmode_t mode,
	unsigned int cmd, unsigned long arg)
{
	struct loop_device *lo = bdev->bd_disk->private_data;
	int err;

	mutex_lock_nested(&lo->lo_ctl_mutex, 1);
	switch (cmd) {
	case LOOP_SET_FD:
		err = loop_set_fd(lo, mode, bdev, arg);
		break;
	case LOOP_CHANGE_FD:
		err = loop_change_fd(lo, bdev, arg);
		break;
	case LOOP_CLR_FD:
		/* loop_clr_fd would have unlocked lo_ctl_mutex on success */
		err = loop_clr_fd(lo);
		if (!err)
			goto out_unlocked;
		break;
	case LOOP_SET_STATUS:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_status_old(lo,
					(struct loop_info __user *)arg);
		break;
	case LOOP_GET_STATUS:
		err = loop_get_status_old(lo, (struct loop_info __user *) arg);
		break;
	case LOOP_SET_STATUS64:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_status64(lo,
					(struct loop_info64 __user *) arg);
		break;
	case LOOP_GET_STATUS64:
		err = loop_get_status64(lo, (struct loop_info64 __user *) arg);
		break;
	case LOOP_SET_CAPACITY:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_capacity(lo);
		break;
	case LOOP_SET_DIRECT_IO:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_dio(lo, arg);
		break;
	case LOOP_SET_BLOCK_SIZE:
		err = -EPERM;
		if ((mode & FMODE_WRITE) || capable(CAP_SYS_ADMIN))
			err = loop_set_block_size(lo, arg);
		break;
	default:
		err = lo->ioctl ? lo->ioctl(lo, cmd, arg) : -EINVAL;
	}
	mutex_unlock(&lo->lo_ctl_mutex);

out_unlocked:
	return err;
}
