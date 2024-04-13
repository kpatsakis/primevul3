ofputil_netmask_to_wcbits(ovs_be32 netmask)
{
    return 32 - ip_count_cidr_bits(netmask);
}
