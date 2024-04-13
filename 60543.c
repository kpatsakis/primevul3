hash_cached_sock_errs(const struct cached_sock_errs_entry *e)
{
	/* Use Murmur3's 32-bit finalizer as an integer hash function */
	DWORD h = e->code;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}
