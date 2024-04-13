static inline int route4_hash_iif(int iif)
{
	return 16 + ((iif >> 16) & 0xF);
}