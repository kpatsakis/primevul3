kvm_pfn_t gfn_to_pfn_atomic(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_pfn_memslot_atomic(gfn_to_memslot(kvm, gfn), gfn);
}
