blkcg_css_alloc(struct cgroup_subsys_state *parent_css)
{
	struct blkcg *blkcg;
	struct cgroup_subsys_state *ret;
	int i;

	mutex_lock(&blkcg_pol_mutex);

	if (!parent_css) {
		blkcg = &blkcg_root;
	} else {
		blkcg = kzalloc(sizeof(*blkcg), GFP_KERNEL);
		if (!blkcg) {
			ret = ERR_PTR(-ENOMEM);
			goto free_blkcg;
		}
	}

	for (i = 0; i < BLKCG_MAX_POLS ; i++) {
		struct blkcg_policy *pol = blkcg_policy[i];
		struct blkcg_policy_data *cpd;

		/*
		 * If the policy hasn't been attached yet, wait for it
		 * to be attached before doing anything else. Otherwise,
		 * check if the policy requires any specific per-cgroup
		 * data: if it does, allocate and initialize it.
		 */
		if (!pol || !pol->cpd_alloc_fn)
			continue;

		cpd = pol->cpd_alloc_fn(GFP_KERNEL);
		if (!cpd) {
			ret = ERR_PTR(-ENOMEM);
			goto free_pd_blkcg;
		}
		blkcg->cpd[i] = cpd;
		cpd->blkcg = blkcg;
		cpd->plid = i;
		if (pol->cpd_init_fn)
			pol->cpd_init_fn(cpd);
	}

	spin_lock_init(&blkcg->lock);
	INIT_RADIX_TREE(&blkcg->blkg_tree, GFP_NOWAIT | __GFP_NOWARN);
	INIT_HLIST_HEAD(&blkcg->blkg_list);
#ifdef CONFIG_CGROUP_WRITEBACK
	INIT_LIST_HEAD(&blkcg->cgwb_list);
#endif
	list_add_tail(&blkcg->all_blkcgs_node, &all_blkcgs);

	mutex_unlock(&blkcg_pol_mutex);
	return &blkcg->css;

free_pd_blkcg:
	for (i--; i >= 0; i--)
		if (blkcg->cpd[i])
			blkcg_policy[i]->cpd_free_fn(blkcg->cpd[i]);
free_blkcg:
	kfree(blkcg);
	mutex_unlock(&blkcg_pol_mutex);
	return ret;
}
