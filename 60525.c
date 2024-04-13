evutil_memclear_(void *mem, size_t len)
{
	evutil_memset_volatile_(mem, 0, len);
}
