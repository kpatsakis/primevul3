static int xor_init(struct loop_device *lo, const struct loop_info64 *info)
{
	if (unlikely(info->lo_encrypt_key_size <= 0))
		return -EINVAL;
	return 0;
}
