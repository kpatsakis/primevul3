static inline bool vmacache_valid_mm(struct mm_struct *mm)
{
	return current->mm == mm && !(current->flags & PF_KTHREAD);
}
