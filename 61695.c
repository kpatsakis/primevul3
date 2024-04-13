sshkey_try_load_public(struct sshkey *k, const char *filename, char **commentp)
{
	FILE *f;
	char line[SSH_MAX_PUBKEY_BYTES];
	char *cp;
	u_long linenum = 0;
	int r;

	if (commentp != NULL)
		*commentp = NULL;
	if ((f = fopen(filename, "r")) == NULL)
		return SSH_ERR_SYSTEM_ERROR;
	while (read_keyfile_line(f, filename, line, sizeof(line),
		    &linenum) != -1) {
		cp = line;
		switch (*cp) {
		case '#':
		case '\n':
		case '\0':
			continue;
		}
		/* Abort loading if this looks like a private key */
		if (strncmp(cp, "-----BEGIN", 10) == 0 ||
		    strcmp(cp, "SSH PRIVATE KEY FILE") == 0)
			break;
		/* Skip leading whitespace. */
		for (; *cp && (*cp == ' ' || *cp == '\t'); cp++)
			;
		if (*cp) {
			if ((r = sshkey_read(k, &cp)) == 0) {
				cp[strcspn(cp, "\r\n")] = '\0';
				if (commentp) {
					*commentp = strdup(*cp ?
					    cp : filename);
					if (*commentp == NULL)
						r = SSH_ERR_ALLOC_FAIL;
				}
				fclose(f);
				return r;
			}
		}
	}
	fclose(f);
	return SSH_ERR_INVALID_FORMAT;
}
