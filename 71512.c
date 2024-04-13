static inline unsigned long round_hint_to_min(unsigned long hint)
{
	hint &= PAGE_MASK;
	if (((void *)hint != NULL) &&
	    (hint < mmap_min_addr))
		return PAGE_ALIGN(mmap_min_addr);
	return hint;
}
