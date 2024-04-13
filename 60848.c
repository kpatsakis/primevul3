static int get_user_page_nowait(unsigned long start, int write,
		struct page **page)
{
	int flags = FOLL_NOWAIT | FOLL_HWPOISON;

	if (write)
		flags |= FOLL_WRITE;

	return get_user_pages(start, 1, flags, page, NULL);
}
