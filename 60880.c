static int kvm_init_debug(void)
{
	int r = -EEXIST;
	struct kvm_stats_debugfs_item *p;

	kvm_debugfs_dir = debugfs_create_dir("kvm", NULL);
	if (kvm_debugfs_dir == NULL)
		goto out;

	kvm_debugfs_num_entries = 0;
	for (p = debugfs_entries; p->name; ++p, kvm_debugfs_num_entries++) {
		if (!debugfs_create_file(p->name, 0444, kvm_debugfs_dir,
					 (void *)(long)p->offset,
					 stat_fops[p->kind]))
			goto out_dir;
	}

	return 0;

out_dir:
	debugfs_remove_recursive(kvm_debugfs_dir);
out:
	return r;
}
