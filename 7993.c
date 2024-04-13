static inline u32 to_hash(u32 id)
{
	u32 h = id & 0xFF;

	if (id & 0x8000)
		h += 256;
	return h;
}