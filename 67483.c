DeleteDNS(short family, wchar_t *if_name)
{
    wchar_t *proto = (family == AF_INET6) ? L"ipv6" : L"ip";
    return netsh_dns_cmd(L"delete", proto, if_name, NULL);
}
