static int vcpu_stat_get_per_vm_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, vcpu_stat_get_per_vm,
				 vcpu_stat_clear_per_vm, "%llu\n");
}
