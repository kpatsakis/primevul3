sshkey_save_private(struct sshkey *key, const char *filename,
    const char *passphrase, const char *comment,
    int force_new_format, const char *new_format_cipher, int new_format_rounds)
{
	struct sshbuf *keyblob = NULL;
	int r;

	if ((keyblob = sshbuf_new()) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	if ((r = sshkey_private_to_fileblob(key, keyblob, passphrase, comment,
	    force_new_format, new_format_cipher, new_format_rounds)) != 0)
		goto out;
	if ((r = sshkey_save_private_blob(keyblob, filename)) != 0)
		goto out;
	r = 0;
 out:
	sshbuf_free(keyblob);
	return r;
}
