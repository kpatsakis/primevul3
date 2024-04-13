eviction_group_priority(size_t n_rules)
{
    uint16_t size = MIN(UINT16_MAX, n_rules);
    return (size << 16) | random_uint16();
}
