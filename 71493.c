struct anon_vma *find_mergeable_anon_vma(struct vm_area_struct *vma)
{
	struct anon_vma *anon_vma;
	struct vm_area_struct *near;

	near = vma->vm_next;
	if (!near)
		goto try_prev;

	anon_vma = reusable_anon_vma(near, vma, near);
	if (anon_vma)
		return anon_vma;
try_prev:
	near = vma->vm_prev;
	if (!near)
		goto none;

	anon_vma = reusable_anon_vma(near, near, vma);
	if (anon_vma)
		return anon_vma;
none:
	/*
	 * There's no absolute need to look only at touching neighbours:
	 * we could search further afield for "compatible" anon_vmas.
	 * But it would probably just be a waste of time searching,
	 * or lead to too many vmas hanging off the same anon_vma.
	 * We're trying to allow mprotect remerging later on,
	 * not trying to minimize memory used for anon_vmas.
	 */
	return NULL;
}
