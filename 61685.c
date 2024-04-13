sshkey_load_cert(const char *filename, struct sshkey **keyp)
{
	struct sshkey *pub = NULL;
	char *file = NULL;
	int r = SSH_ERR_INTERNAL_ERROR;

	if (keyp != NULL)
		*keyp = NULL;

	if (asprintf(&file, "%s-cert.pub", filename) == -1)
		return SSH_ERR_ALLOC_FAIL;

	if ((pub = sshkey_new(KEY_UNSPEC)) == NULL) {
		goto out;
	}
	if ((r = sshkey_try_load_public(pub, file, NULL)) != 0)
		goto out;
	/* success */
	if (keyp != NULL) {
		*keyp = pub;
		pub = NULL;
	}
	r = 0;
 out:
	free(file);
	sshkey_free(pub);
	return r;
}
