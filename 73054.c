static inline void tun_flow_save_rps_rxhash(struct tun_flow_entry *e, u32 hash)
{
	if (unlikely(e->rps_rxhash != hash))
		e->rps_rxhash = hash;
}
