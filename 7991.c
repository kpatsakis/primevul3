static inline int route4_hash_to(u32 id)
{
	return id & 0xFF;
}