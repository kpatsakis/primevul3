virDomainSetAutostart(virDomainPtr domain,
                      int autostart)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "autostart=%d", autostart);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainSetAutostart) {
        int ret;
        ret = conn->driver->domainSetAutostart(domain, autostart);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
