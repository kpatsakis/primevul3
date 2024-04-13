static int vcpu_stat_clear_per_vm(void *data, u64 val)
{
	int i;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;
	struct kvm_vcpu *vcpu;

	if (val)
		return -EINVAL;

	kvm_for_each_vcpu(i, vcpu, stat_data->kvm)
		*(u64 *)((void *)vcpu + stat_data->offset) = 0;

	return 0;
}
