DLLIMPORT char *cfg_searchpath(cfg_searchpath_t *p, const char *file)
{
	char *fullpath;
#ifdef HAVE_SYS_STAT_H
	struct stat st;
	int err;
#endif

	if (!p || !file) {
		errno = EINVAL;
		return NULL;
	}

	if (file[0] == '/') {
		fullpath = strdup(file);
		if (!fullpath)
			return NULL;
		goto check;
	}

	if ((fullpath = cfg_searchpath(p->next, file)) != NULL)
		return fullpath;

	if ((fullpath = cfg_make_fullpath(p->dir, file)) == NULL)
		return NULL;

check:
#ifdef HAVE_SYS_STAT_H
	err = stat((const char *)fullpath, &st);
	if ((!err) && S_ISREG(st.st_mode))
		return fullpath;
#else
	/* needs an alternative check here for win32 */
#endif

	free(fullpath);
	return NULL;
}