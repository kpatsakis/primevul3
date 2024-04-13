static int kvm_vcpu_check_block(struct kvm_vcpu *vcpu)
{
	if (kvm_arch_vcpu_runnable(vcpu)) {
		kvm_make_request(KVM_REQ_UNHALT, vcpu);
		return -EINTR;
	}
	if (kvm_cpu_has_pending_timer(vcpu))
		return -EINTR;
	if (signal_pending(current))
		return -EINTR;

	return 0;
}
