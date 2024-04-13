sshkey_load_private(const char *filename, const char *passphrase,
    struct sshkey **keyp, char **commentp)
{
	struct sshbuf *buffer = NULL;
	int r, fd;

	if (keyp != NULL)
		*keyp = NULL;
	if (commentp != NULL)
		*commentp = NULL;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return SSH_ERR_SYSTEM_ERROR;
	if (sshkey_perm_ok(fd, filename) != 0) {
		r = SSH_ERR_KEY_BAD_PERMISSIONS;
		goto out;
	}

	if ((buffer = sshbuf_new()) == NULL) {
		r = SSH_ERR_ALLOC_FAIL;
		goto out;
	}
	if ((r = sshkey_load_file(fd, buffer)) != 0 ||
	    (r = sshkey_parse_private_fileblob(buffer, passphrase, keyp,
	    commentp)) != 0)
		goto out;
	r = 0;
 out:
	close(fd);
	sshbuf_free(buffer);
	return r;
}
