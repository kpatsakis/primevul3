InterfaceLuid(const char *iface_name, PNET_LUID luid)
{
    NETIO_STATUS status;
    LPWSTR wide_name = utf8to16(iface_name);

    if (wide_name)
    {
        status = ConvertInterfaceAliasToLuid(wide_name, luid);
        free(wide_name);
    }
    else
    {
        status = ERROR_OUTOFMEMORY;
    }
    return status;
}
