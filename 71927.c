void vcpu_load(struct kvm_vcpu *vcpu)
{
	int cpu = get_cpu();
	preempt_notifier_register(&vcpu->preempt_notifier);
	kvm_arch_vcpu_load(vcpu, cpu);
	put_cpu();
}
