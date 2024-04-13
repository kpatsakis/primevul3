virDomainGetHostname(virDomainPtr domain, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    conn = domain->conn;

    if (conn->driver->domainGetHostname) {
        char *ret;
        ret = conn->driver->domainGetHostname(domain, flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
