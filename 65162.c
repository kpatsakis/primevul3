check_authkeys_file(struct ssh *ssh, struct passwd *pw, FILE *f,
    char *file, struct sshkey *key, struct sshauthopt **authoptsp)
{
	char *cp, *line = NULL, loc[256];
	size_t linesize = 0;
	int found_key = 0;
	u_long linenum = 0;

	if (authoptsp != NULL)
		*authoptsp = NULL;

	while (getline(&line, &linesize, f) != -1) {
		linenum++;
		/* Always consume entire file */
		if (found_key)
			continue;

		/* Skip leading whitespace, empty and comment lines. */
		cp = line;
		skip_space(&cp);
		if (!*cp || *cp == '\n' || *cp == '#')
			continue;
		snprintf(loc, sizeof(loc), "%.200s:%lu", file, linenum);
		if (check_authkey_line(ssh, pw, key, cp, loc, authoptsp) == 0)
			found_key = 1;
	}
	free(line);
	return found_key;
}
