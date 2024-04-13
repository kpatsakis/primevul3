virDomainCreateLinux(virConnectPtr conn, const char *xmlDesc,
                     unsigned int flags)
{
    return virDomainCreateXML(conn, xmlDesc, flags);
}
