static int vm_stat_get_per_vm_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, vm_stat_get_per_vm,
				vm_stat_clear_per_vm, "%llu\n");
}
