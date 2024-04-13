DLLIMPORT int cfg_add_searchpath(cfg_t *cfg, const char *dir)
{
	cfg_searchpath_t *p;
	char *d;

	if (!cfg || !dir) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	d = cfg_tilde_expand(dir);
	if (!d)
		return CFG_FAIL;

	p = malloc(sizeof(cfg_searchpath_t));
	if (!p) {
		free(d);
		return CFG_FAIL;
	}

	p->next   = cfg->path;
	p->dir    = d;
	cfg->path = p;

	return CFG_SUCCESS;
}