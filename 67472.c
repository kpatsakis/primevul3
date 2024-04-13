AddDNS(short family, wchar_t *if_name, wchar_t *addr)
{
    wchar_t *proto = (family == AF_INET6) ? L"ipv6" : L"ip";
    return netsh_dns_cmd(L"add", proto, if_name, addr);
}
