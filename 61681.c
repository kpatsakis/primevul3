gids_cache_purge(void)
{
	int i;
	gids_cache_t *p, *q;

	slurm_mutex_lock(&gids_mutex);
	for (i=0; i<GIDS_HASH_LEN; i++) {
		p = gids_hashtbl[i];
		while (p) {
			q = p->next;
			_dealloc_gids_cache(p);
			p = q;
		}
		gids_hashtbl[i] = NULL;
	}
	slurm_mutex_unlock(&gids_mutex);
}
