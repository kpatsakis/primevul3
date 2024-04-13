static int vcpu_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
	struct kvm_stat_data stat_tmp = {.offset = offset};
	u64 tmp_val;

	*val = 0;
	spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
		stat_tmp.kvm = kvm;
		vcpu_stat_get_per_vm((void *)&stat_tmp, &tmp_val);
		*val += tmp_val;
	}
	spin_unlock(&kvm_lock);
	return 0;
}
