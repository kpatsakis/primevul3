static inline int task_fits_capacity(struct task_struct *p, long capacity)
{
	return capacity * 1024 > task_util_est(p) * capacity_margin;
}
