static inline u32 tun_hashfn(u32 rxhash)
{
	return rxhash & 0x3ff;
}
