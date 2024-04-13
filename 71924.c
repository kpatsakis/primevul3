static int kvm_vm_ioctl_enable_cap_generic(struct kvm *kvm,
					   struct kvm_enable_cap *cap)
{
	switch (cap->cap) {
#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
	case KVM_CAP_MANUAL_DIRTY_LOG_PROTECT:
		if (cap->flags || (cap->args[0] & ~1))
			return -EINVAL;
		kvm->manual_dirty_log_protect = cap->args[0];
		return 0;
#endif
	default:
		return kvm_vm_ioctl_enable_cap(kvm, cap);
	}
}
