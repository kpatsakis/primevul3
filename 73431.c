virDomainGetControlInfo(virDomainPtr domain,
                        virDomainControlInfoPtr info,
                        unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "info=%p, flags=%x", info, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(info, error);

    conn = domain->conn;
    if (conn->driver->domainGetControlInfo) {
        int ret;
        ret = conn->driver->domainGetControlInfo(domain, info, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
