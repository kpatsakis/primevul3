bool kvm_is_reserved_pfn(kvm_pfn_t pfn)
{
	if (pfn_valid(pfn))
		return PageReserved(pfn_to_page(pfn));

	return true;
}
