static inline void put_numa_group(struct numa_group *grp)
{
	if (atomic_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}
