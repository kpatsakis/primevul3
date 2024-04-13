_alloc_gids_cache(char *user, gid_t gid, gids_t *gids, gids_cache_t *next)
{
	gids_cache_t *p;

	p = (gids_cache_t *)xmalloc(sizeof(gids_cache_t));
	p->user = xstrdup(user);
	p->timestamp = time(NULL);
	p->gid = gid;
	p->gids = gids;
	p->next = next;
	return p;
}
