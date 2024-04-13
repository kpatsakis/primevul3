static int kvm_create_vcpu_debugfs(struct kvm_vcpu *vcpu)
{
	char dir_name[ITOA_MAX_LEN * 2];
	int ret;

	if (!kvm_arch_has_vcpu_debugfs())
		return 0;

	if (!debugfs_initialized())
		return 0;

	snprintf(dir_name, sizeof(dir_name), "vcpu%d", vcpu->vcpu_id);
	vcpu->debugfs_dentry = debugfs_create_dir(dir_name,
								vcpu->kvm->debugfs_dentry);
	if (!vcpu->debugfs_dentry)
		return -ENOMEM;

	ret = kvm_arch_create_vcpu_debugfs(vcpu);
	if (ret < 0) {
		debugfs_remove_recursive(vcpu->debugfs_dentry);
		return ret;
	}

	return 0;
}
