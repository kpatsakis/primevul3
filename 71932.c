static int vm_stat_clear_per_vm(void *data, u64 val)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;

	if (val)
		return -EINVAL;

	*(ulong *)((void *)stat_data->kvm + stat_data->offset) = 0;

	return 0;
}
