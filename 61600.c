_gids_dup(gids_t *g)
{
	int buf_size;
	gids_t *n = xmalloc(sizeof(gids_t));
	n->ngids = g->ngids;
	buf_size = g->ngids * sizeof(gid_t);
	n->gids = xmalloc(buf_size);
	memcpy(n->gids, g->gids, buf_size);
	return n;
}
