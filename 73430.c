virDomainGetConnect(virDomainPtr dom)
{
    VIR_DOMAIN_DEBUG(dom);

    virResetLastError();

    virCheckDomainReturn(dom, NULL);

    return dom->conn;
}
