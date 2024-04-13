static int show_smaps_rollup(struct seq_file *m, void *v)
{
	struct proc_maps_private *priv = m->private;
	struct mem_size_stats mss;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	unsigned long last_vma_end = 0;
	int ret = 0;

	priv->task = get_proc_task(priv->inode);
	if (!priv->task)
		return -ESRCH;

	mm = priv->mm;
	if (!mm || !mmget_not_zero(mm)) {
		ret = -ESRCH;
		goto out_put_task;
	}

	memset(&mss, 0, sizeof(mss));

	down_read(&mm->mmap_sem);
	hold_task_mempolicy(priv);

	for (vma = priv->mm->mmap; vma; vma = vma->vm_next) {
		smap_gather_stats(vma, &mss);
		last_vma_end = vma->vm_end;
	}

	show_vma_header_prefix(m, priv->mm->mmap->vm_start,
			       last_vma_end, 0, 0, 0, 0);
	seq_pad(m, ' ');
	seq_puts(m, "[rollup]\n");

	__show_smap(m, &mss);

	release_task_mempolicy(priv);
	up_read(&mm->mmap_sem);
	mmput(mm);

out_put_task:
	put_task_struct(priv->task);
	priv->task = NULL;

	return ret;
}
