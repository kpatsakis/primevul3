virDomainInjectNMI(virDomainPtr domain, unsigned int flags)
{
    virConnectPtr conn;
    VIR_DOMAIN_DEBUG(domain, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainInjectNMI) {
        int ret;
        ret = conn->driver->domainInjectNMI(domain, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
