_dealloc_gids_cache(gids_cache_t *p)
{
	xfree(p->user);
	_dealloc_gids(p->gids);
	xfree(p);
}
