static ssize_t loop_attr_backing_file_show(struct loop_device *lo, char *buf)
{
	ssize_t ret;
	char *p = NULL;

	spin_lock_irq(&lo->lo_lock);
	if (lo->lo_backing_file)
		p = file_path(lo->lo_backing_file, buf, PAGE_SIZE - 1);
	spin_unlock_irq(&lo->lo_lock);

	if (IS_ERR_OR_NULL(p))
		ret = PTR_ERR(p);
	else {
		ret = strlen(p);
		memmove(buf, p, ret);
		buf[ret++] = '\n';
		buf[ret] = 0;
	}

	return ret;
}
