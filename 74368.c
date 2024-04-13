static int f_hidg_release(struct inode *inode, struct file *fd)
{
	fd->private_data = NULL;
	return 0;
}
