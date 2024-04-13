static long loop_control_ioctl(struct file *file, unsigned int cmd,
			       unsigned long parm)
{
	struct loop_device *lo;
	int ret = -ENOSYS;

	mutex_lock(&loop_index_mutex);
	switch (cmd) {
	case LOOP_CTL_ADD:
		ret = loop_lookup(&lo, parm);
		if (ret >= 0) {
			ret = -EEXIST;
			break;
		}
		ret = loop_add(&lo, parm);
		break;
	case LOOP_CTL_REMOVE:
		ret = loop_lookup(&lo, parm);
		if (ret < 0)
			break;
		mutex_lock(&lo->lo_ctl_mutex);
		if (lo->lo_state != Lo_unbound) {
			ret = -EBUSY;
			mutex_unlock(&lo->lo_ctl_mutex);
			break;
		}
		if (atomic_read(&lo->lo_refcnt) > 0) {
			ret = -EBUSY;
			mutex_unlock(&lo->lo_ctl_mutex);
			break;
		}
		lo->lo_disk->private_data = NULL;
		mutex_unlock(&lo->lo_ctl_mutex);
		idr_remove(&loop_index_idr, lo->lo_number);
		loop_remove(lo);
		break;
	case LOOP_CTL_GET_FREE:
		ret = loop_lookup(&lo, -1);
		if (ret >= 0)
			break;
		ret = loop_add(&lo, -1);
	}
	mutex_unlock(&loop_index_mutex);

	return ret;
}
