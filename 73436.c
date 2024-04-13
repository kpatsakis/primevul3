virDomainGetInfo(virDomainPtr domain, virDomainInfoPtr info)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "info=%p", info);

    virResetLastError();

    if (info)
        memset(info, 0, sizeof(*info));

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(info, error);

    conn = domain->conn;

    if (conn->driver->domainGetInfo) {
        int ret;
        ret = conn->driver->domainGetInfo(domain, info);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
