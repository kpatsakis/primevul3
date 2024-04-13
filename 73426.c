virDomainGetAutostart(virDomainPtr domain,
                      int *autostart)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "autostart=%p", autostart);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(autostart, error);

    conn = domain->conn;

    if (conn->driver->domainGetAutostart) {
        int ret;
        ret = conn->driver->domainGetAutostart(domain, autostart);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
