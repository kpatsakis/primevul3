xfs_vm_readpages(
	struct file		*unused,
	struct address_space	*mapping,
	struct list_head	*pages,
	unsigned		nr_pages)
{
	trace_xfs_vm_readpages(mapping->host, nr_pages);
	return mpage_readpages(mapping, pages, nr_pages, xfs_get_blocks);
}
