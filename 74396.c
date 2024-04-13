static int anon_pipe_buf_steal(struct pipe_inode_info *pipe,
			       struct pipe_buffer *buf)
{
	struct page *page = buf->page;

	if (page_count(page) == 1) {
		if (memcg_kmem_enabled())
			memcg_kmem_uncharge(page, 0);
		__SetPageLocked(page);
		return 0;
	}
	return 1;
}
