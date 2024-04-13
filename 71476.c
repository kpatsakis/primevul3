static int browse_rb(struct mm_struct *mm)
{
	struct rb_root *root = &mm->mm_rb;
	int i = 0, j, bug = 0;
	struct rb_node *nd, *pn = NULL;
	unsigned long prev = 0, pend = 0;

	for (nd = rb_first(root); nd; nd = rb_next(nd)) {
		struct vm_area_struct *vma;
		vma = rb_entry(nd, struct vm_area_struct, vm_rb);
		if (vma->vm_start < prev) {
			pr_emerg("vm_start %lx < prev %lx\n",
				  vma->vm_start, prev);
			bug = 1;
		}
		if (vma->vm_start < pend) {
			pr_emerg("vm_start %lx < pend %lx\n",
				  vma->vm_start, pend);
			bug = 1;
		}
		if (vma->vm_start > vma->vm_end) {
			pr_emerg("vm_start %lx > vm_end %lx\n",
				  vma->vm_start, vma->vm_end);
			bug = 1;
		}
		spin_lock(&mm->page_table_lock);
		if (vma->rb_subtree_gap != vma_compute_subtree_gap(vma)) {
			pr_emerg("free gap %lx, correct %lx\n",
			       vma->rb_subtree_gap,
			       vma_compute_subtree_gap(vma));
			bug = 1;
		}
		spin_unlock(&mm->page_table_lock);
		i++;
		pn = nd;
		prev = vma->vm_start;
		pend = vma->vm_end;
	}
	j = 0;
	for (nd = pn; nd; nd = rb_prev(nd))
		j++;
	if (i != j) {
		pr_emerg("backwards %d, forwards %d\n", j, i);
		bug = 1;
	}
	return bug ? -1 : i;
}
