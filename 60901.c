static void kvm_sched_in(struct preempt_notifier *pn, int cpu)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);

	if (vcpu->preempted)
		vcpu->preempted = false;

	kvm_arch_sched_in(vcpu, cpu);

	kvm_arch_vcpu_load(vcpu, cpu);
}
