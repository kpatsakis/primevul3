static int kvm_dev_ioctl_create_vm(unsigned long type)
{
	int r;
	struct kvm *kvm;
	struct file *file;

	kvm = kvm_create_vm(type);
	if (IS_ERR(kvm))
		return PTR_ERR(kvm);
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	r = kvm_coalesced_mmio_init(kvm);
	if (r < 0) {
		kvm_put_kvm(kvm);
		return r;
	}
#endif
	r = get_unused_fd_flags(O_CLOEXEC);
	if (r < 0) {
		kvm_put_kvm(kvm);
		return r;
	}
	file = anon_inode_getfile("kvm-vm", &kvm_vm_fops, kvm, O_RDWR);
	if (IS_ERR(file)) {
		put_unused_fd(r);
		kvm_put_kvm(kvm);
		return PTR_ERR(file);
	}

	if (kvm_create_vm_debugfs(kvm, r) < 0) {
		put_unused_fd(r);
		fput(file);
		return -ENOMEM;
	}

	fd_install(r, file);
	return r;
}
