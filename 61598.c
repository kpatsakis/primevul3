static gids_t *_gids_cache_lookup(char *user, gid_t gid)
{
	size_t idx;
	gids_cache_t *p;
	bool found_but_old = false;
	time_t now = 0;
	int ngroups = NGROUPS_START;
	gid_t *groups;
	gids_t *ret_gids = NULL;

	idx = _gids_hashtbl_idx(user);
	slurm_mutex_lock(&gids_mutex);
	p = gids_hashtbl[idx];
	while (p) {
		if (xstrcmp(p->user, user) == 0 && p->gid == gid) {
			slurm_ctl_conf_t *cf = slurm_conf_lock();
			int group_ttl = cf->group_info & GROUP_TIME_MASK;
			slurm_conf_unlock();
			if (!group_ttl) {
				ret_gids = _gids_dup(p->gids);
				goto done;
			}
			now = time(NULL);
			if (difftime(now, p->timestamp) < group_ttl) {
				ret_gids = _gids_dup(p->gids);
				goto done;
			} else {
				found_but_old = true;
				break;
			}
		}
		p = p->next;
	}
	/* Cache lookup failed or cached value was too old, fetch new
	 * value and insert it into cache.  */
	groups = xmalloc(ngroups * sizeof(gid_t));
	while (getgrouplist(user, gid, groups, &ngroups) == -1) {
		/* group list larger than array, resize array to fit */
		groups = xrealloc(groups, ngroups * sizeof(gid_t));
	}
	if (found_but_old) {
		xfree(p->gids->gids);
		p->gids->gids = groups;
		p->gids->ngids = ngroups;
		p->timestamp = now;
		ret_gids = _gids_dup(p->gids);
	} else {
		gids_t *gids = _alloc_gids(ngroups, groups);
		_gids_cache_register(user, gid, gids);
		ret_gids = _gids_dup(gids);
	}
done:
	slurm_mutex_unlock(&gids_mutex);
	return ret_gids;
}
