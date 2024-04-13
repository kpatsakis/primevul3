static inline bool tun_is_little_endian(struct tun_struct *tun)
{
	return tun->flags & TUN_VNET_LE ||
		tun_legacy_is_little_endian(tun);
}
