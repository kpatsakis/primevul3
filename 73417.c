virDomainDestroy(virDomainPtr domain)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainDestroy) {
        int ret;
        ret = conn->driver->domainDestroy(domain);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
