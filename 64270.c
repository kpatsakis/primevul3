hash_cookie(ovs_be64 cookie)
{
    return hash_uint64((OVS_FORCE uint64_t)cookie);
}
