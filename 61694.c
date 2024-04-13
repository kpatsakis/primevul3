sshkey_save_private_blob(struct sshbuf *keybuf, const char *filename)
{
	int fd, oerrno;

	if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0)
		return SSH_ERR_SYSTEM_ERROR;
	if (atomicio(vwrite, fd, (u_char *)sshbuf_ptr(keybuf),
	    sshbuf_len(keybuf)) != sshbuf_len(keybuf)) {
		oerrno = errno;
		close(fd);
		unlink(filename);
		errno = oerrno;
		return SSH_ERR_SYSTEM_ERROR;
	}
	close(fd);
	return 0;
}
