static int vm_stat_clear(void *_offset, u64 val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
	struct kvm_stat_data stat_tmp = {.offset = offset};

	if (val)
		return -EINVAL;

	spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
		stat_tmp.kvm = kvm;
		vm_stat_clear_per_vm((void *)&stat_tmp, 0);
	}
	spin_unlock(&kvm_lock);

	return 0;
}
