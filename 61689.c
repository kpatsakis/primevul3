sshkey_load_private_type_fd(int fd, int type, const char *passphrase,
    struct sshkey **keyp, char **commentp)
{
	struct sshbuf *buffer = NULL;
	int r;

	if (keyp != NULL)
		*keyp = NULL;
	if ((buffer = sshbuf_new()) == NULL) {
		r = SSH_ERR_ALLOC_FAIL;
		goto out;
	}
	if ((r = sshkey_load_file(fd, buffer)) != 0 ||
	    (r = sshkey_parse_private_fileblob_type(buffer, type,
	    passphrase, keyp, commentp)) != 0)
		goto out;

	/* success */
	r = 0;
 out:
	sshbuf_free(buffer);
	return r;
}
