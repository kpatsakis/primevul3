static int tun_chr_close(struct inode *inode, struct file *file)
{
	struct tun_file *tfile = file->private_data;

	tun_detach(tfile, true);

	return 0;
}
