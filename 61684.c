sshkey_in_file(struct sshkey *key, const char *filename, int strict_type,
    int check_ca)
{
	FILE *f;
	char line[SSH_MAX_PUBKEY_BYTES];
	char *cp;
	u_long linenum = 0;
	int r = 0;
	struct sshkey *pub = NULL;
	int (*sshkey_compare)(const struct sshkey *, const struct sshkey *) =
	    strict_type ?  sshkey_equal : sshkey_equal_public;

	if ((f = fopen(filename, "r")) == NULL)
		return SSH_ERR_SYSTEM_ERROR;

	while (read_keyfile_line(f, filename, line, sizeof(line),
	    &linenum) != -1) {
		cp = line;

		/* Skip leading whitespace. */
		for (; *cp && (*cp == ' ' || *cp == '\t'); cp++)
			;

		/* Skip comments and empty lines */
		switch (*cp) {
		case '#':
		case '\n':
		case '\0':
			continue;
		}

		if ((pub = sshkey_new(KEY_UNSPEC)) == NULL) {
			r = SSH_ERR_ALLOC_FAIL;
			goto out;
		}
		if ((r = sshkey_read(pub, &cp)) != 0)
			goto out;
		if (sshkey_compare(key, pub) ||
		    (check_ca && sshkey_is_cert(key) &&
		    sshkey_compare(key->cert->signature_key, pub))) {
			r = 0;
			goto out;
		}
		sshkey_free(pub);
		pub = NULL;
	}
	r = SSH_ERR_KEY_NOT_FOUND;
 out:
	sshkey_free(pub);
	fclose(f);
	return r;
}
