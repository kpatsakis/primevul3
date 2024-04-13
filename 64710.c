static bool vmacache_valid(struct mm_struct *mm)
{
	struct task_struct *curr;

	if (!vmacache_valid_mm(mm))
		return false;

	curr = current;
	if (mm->vmacache_seqnum != curr->vmacache.seqnum) {
		/*
		 * First attempt will always be invalid, initialize
		 * the new cache for this task here.
		 */
		curr->vmacache.seqnum = mm->vmacache_seqnum;
		vmacache_flush(curr);
		return false;
	}
	return true;
}
