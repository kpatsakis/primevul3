static int available_error_type_open(struct inode *inode, struct file *file)
{
	return single_open(file, available_error_type_show, NULL);
}
