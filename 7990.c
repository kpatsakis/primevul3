static inline int route4_hash_from(u32 id)
{
	return (id >> 16) & 0xF;
}