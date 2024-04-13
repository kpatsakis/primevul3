void kvm_vcpu_wake_up(struct kvm_vcpu *vcpu)
{
	struct swait_queue_head *wqp;

	wqp = kvm_arch_vcpu_wq(vcpu);
	if (swait_active(wqp)) {
		swake_up(wqp);
		++vcpu->stat.halt_wakeup;
	}

}
