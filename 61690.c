sshkey_load_public(const char *filename, struct sshkey **keyp, char **commentp)
{
	struct sshkey *pub = NULL;
	char file[PATH_MAX];
	int r, fd;

	if (keyp != NULL)
		*keyp = NULL;
	if (commentp != NULL)
		*commentp = NULL;

	/* XXX should load file once and attempt to parse each format */

	if ((fd = open(filename, O_RDONLY)) < 0)
		goto skip;
#ifdef WITH_SSH1
	/* try rsa1 private key */
	r = sshkey_load_public_rsa1(fd, keyp, commentp);
	close(fd);
	switch (r) {
	case SSH_ERR_INTERNAL_ERROR:
	case SSH_ERR_ALLOC_FAIL:
	case SSH_ERR_INVALID_ARGUMENT:
	case SSH_ERR_SYSTEM_ERROR:
	case 0:
		return r;
	}
#else /* WITH_SSH1 */
	close(fd);
#endif /* WITH_SSH1 */

	/* try ssh2 public key */
	if ((pub = sshkey_new(KEY_UNSPEC)) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	if ((r = sshkey_try_load_public(pub, filename, commentp)) == 0) {
		if (keyp != NULL)
			*keyp = pub;
		return 0;
	}
	sshkey_free(pub);

#ifdef WITH_SSH1
	/* try rsa1 public key */
	if ((pub = sshkey_new(KEY_RSA1)) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	if ((r = sshkey_try_load_public(pub, filename, commentp)) == 0) {
		if (keyp != NULL)
			*keyp = pub;
		return 0;
	}
	sshkey_free(pub);
#endif /* WITH_SSH1 */

 skip:
	/* try .pub suffix */
	if ((pub = sshkey_new(KEY_UNSPEC)) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	r = SSH_ERR_ALLOC_FAIL;	/* in case strlcpy or strlcat fail */
	if ((strlcpy(file, filename, sizeof file) < sizeof(file)) &&
	    (strlcat(file, ".pub", sizeof file) < sizeof(file)) &&
	    (r = sshkey_try_load_public(pub, file, commentp)) == 0) {
		if (keyp != NULL)
			*keyp = pub;
		return 0;
	}
	sshkey_free(pub);

	return r;
}
