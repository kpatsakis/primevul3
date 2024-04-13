format_address(netdissect_options *ndo, const u_char *prefix)
{
    if(memcmp(prefix, v4prefix, 12) == 0)
        return ipaddr_string(ndo, prefix + 12);
    else
        return ip6addr_string(ndo, prefix);
}
