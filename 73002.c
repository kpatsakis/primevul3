format_prefix(netdissect_options *ndo, const u_char *prefix, unsigned char plen)
{
    static char buf[50];
    if(plen >= 96 && memcmp(prefix, v4prefix, 12) == 0)
        snprintf(buf, 50, "%s/%u", ipaddr_string(ndo, prefix + 12), plen - 96);
    else
        snprintf(buf, 50, "%s/%u", ip6addr_string(ndo, prefix), plen);
    buf[49] = '\0';
    return buf;
}
