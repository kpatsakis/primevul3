should_send_version_bitmap(uint32_t allowed_versions)
{
    return !is_pow2((allowed_versions >> 1) + 1);
}
