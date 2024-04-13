static int cfg_free_searchpath(cfg_searchpath_t *p)
{
	if (p) {
		cfg_free_searchpath(p->next);
		free(p->dir);
		free(p);
	}

	return CFG_SUCCESS;
}