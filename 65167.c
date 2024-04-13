process_principals(struct ssh *ssh, FILE *f, const char *file,
    const struct sshkey_cert *cert, struct sshauthopt **authoptsp)
{
	char loc[256], *line = NULL, *cp, *ep;
	size_t linesize = 0;
	u_long linenum = 0;
	u_int found_principal = 0;

	if (authoptsp != NULL)
		*authoptsp = NULL;

	while (getline(&line, &linesize, f) != -1) {
		linenum++;
		/* Always consume entire input */
		if (found_principal)
			continue;

		/* Skip leading whitespace. */
		for (cp = line; *cp == ' ' || *cp == '\t'; cp++)
			;
		/* Skip blank and comment lines. */
		if ((ep = strchr(cp, '#')) != NULL)
			*ep = '\0';
		if (!*cp || *cp == '\n')
			continue;

		snprintf(loc, sizeof(loc), "%.200s:%lu", file, linenum);
		if (check_principals_line(ssh, cp, cert, loc, authoptsp) == 0)
			found_principal = 1;
	}
	free(line);
	return found_principal;
}
