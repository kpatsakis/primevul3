sshkey_load_private_type(int type, const char *filename, const char *passphrase,
    struct sshkey **keyp, char **commentp, int *perm_ok)
{
	int fd, r;

	if (keyp != NULL)
		*keyp = NULL;
	if (commentp != NULL)
		*commentp = NULL;

	if ((fd = open(filename, O_RDONLY)) < 0) {
		if (perm_ok != NULL)
			*perm_ok = 0;
		return SSH_ERR_SYSTEM_ERROR;
	}
	if (sshkey_perm_ok(fd, filename) != 0) {
		if (perm_ok != NULL)
			*perm_ok = 0;
		r = SSH_ERR_KEY_BAD_PERMISSIONS;
		goto out;
	}
	if (perm_ok != NULL)
		*perm_ok = 1;

	r = sshkey_load_private_type_fd(fd, type, passphrase, keyp, commentp);
 out:
	close(fd);
	return r;
}
