static inline int get_numa_group(struct numa_group *grp)
{
	return atomic_inc_not_zero(&grp->refcount);
}
