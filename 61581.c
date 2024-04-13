_dealloc_gids(gids_t *p)
{
	xfree(p->gids);
	xfree(p);
}
