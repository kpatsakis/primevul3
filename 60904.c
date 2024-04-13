void kvm_set_pfn_dirty(kvm_pfn_t pfn)
{
	if (!kvm_is_reserved_pfn(pfn)) {
		struct page *page = pfn_to_page(pfn);

		if (!PageReserved(page))
			SetPageDirty(page);
	}
}
