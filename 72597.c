static inline struct cfs_bandwidth *tg_cfs_bandwidth(struct task_group *tg)
{
	return &tg->cfs_bandwidth;
}
