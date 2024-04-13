static int vcpu_stat_get_per_vm(void *data, u64 *val)
{
	int i;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;
	struct kvm_vcpu *vcpu;

	*val = 0;

	kvm_for_each_vcpu(i, vcpu, stat_data->kvm)
		*val += *(u64 *)((void *)vcpu + stat_data->offset);

	return 0;
}
