pick_fallback_dpid(void)
{
    struct eth_addr ea;
    eth_addr_nicira_random(&ea);
    return eth_addr_to_uint64(ea);
}
