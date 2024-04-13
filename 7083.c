DLLIMPORT char *cfg_tilde_expand(const char *filename)
{
	char *expanded = NULL;

#ifndef _WIN32
	/* Do tilde expansion */
	if (filename[0] == '~') {
		struct passwd *passwd = NULL;
		const char *file = NULL;

		if (filename[1] == '/' || filename[1] == 0) {
			/* ~ or ~/path */
			passwd = getpwuid(geteuid());
			file = filename + 1;
		} else {
			char *user; /* ~user or ~user/path */
			size_t len;

			file = strchr(filename, '/');
			if (file == NULL)
				file = filename + strlen(filename);

			len = file - filename - 1;
			user = malloc(len + 1);
			if (!user)
				return NULL;

			strncpy(user, &filename[1], len);
			user[len] = 0;
			passwd = getpwnam(user);
			free(user);
		}

		if (passwd) {
			expanded = malloc(strlen(passwd->pw_dir) + strlen(file) + 1);
			if (!expanded)
				return NULL;

			strcpy(expanded, passwd->pw_dir);
			strcat(expanded, file);
		}
	}
#endif
	if (!expanded)
		expanded = strdup(filename);

	return expanded;
}