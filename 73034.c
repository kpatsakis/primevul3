static int tun_chr_fasync(int fd, struct file *file, int on)
{
	struct tun_file *tfile = file->private_data;
	int ret;

	if ((ret = fasync_helper(fd, file, on, &tfile->fasync)) < 0)
		goto out;

	if (on) {
		__f_setown(file, task_pid(current), PIDTYPE_PID, 0);
		tfile->flags |= TUN_FASYNC;
	} else
		tfile->flags &= ~TUN_FASYNC;
	ret = 0;
out:
	return ret;
}
