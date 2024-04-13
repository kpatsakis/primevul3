_gids_cache_register(char *user, gid_t gid, gids_t *gids)
{
	size_t idx;
	gids_cache_t *p, *q;

	idx = _gids_hashtbl_idx(user);
	q = gids_hashtbl[idx];
	p = _alloc_gids_cache(user, gid, gids, q);
	gids_hashtbl[idx] = p;
	debug2("Cached group access list for %s/%d", user, gid);
}
