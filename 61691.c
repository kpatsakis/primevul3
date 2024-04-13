sshkey_load_public_rsa1(int fd, struct sshkey **keyp, char **commentp)
{
	struct sshbuf *b = NULL;
	int r;

	if (keyp != NULL)
		*keyp = NULL;
	if (commentp != NULL)
		*commentp = NULL;

	if ((b = sshbuf_new()) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	if ((r = sshkey_load_file(fd, b)) != 0)
		goto out;
	if ((r = sshkey_parse_public_rsa1_fileblob(b, keyp, commentp)) != 0)
		goto out;
	r = 0;
 out:
	sshbuf_free(b);
	return r;
}
