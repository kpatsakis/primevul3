xfs_vm_invalidatepage(
	struct page		*page,
	unsigned int		offset,
	unsigned int		length)
{
	trace_xfs_invalidatepage(page->mapping->host, page, offset,
				 length);
	block_invalidatepage(page, offset, length);
}
