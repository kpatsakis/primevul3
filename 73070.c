static inline bool tun_legacy_is_little_endian(struct tun_struct *tun)
{
	return tun->flags & TUN_VNET_BE ? false :
		virtio_legacy_is_little_endian();
}
