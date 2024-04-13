_alloc_gids(int n, gid_t *gids)
{
	gids_t *new;

	new = (gids_t *)xmalloc(sizeof(gids_t));
	new->ngids = n;
	new->gids = gids;
	return new;
}
