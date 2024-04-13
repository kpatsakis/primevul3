static inline bool tun_legacy_is_little_endian(struct tun_struct *tun)
{
	return virtio_legacy_is_little_endian();
}
