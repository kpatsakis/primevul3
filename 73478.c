virDomainMigrateConfirm3(virDomainPtr domain,
                         const char *cookiein,
                         int cookieinlen,
                         unsigned long flags,
                         int cancelled)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain,
                     "cookiein=%p, cookieinlen=%d, flags=%lx, cancelled=%d",
                     cookiein, cookieinlen, flags, cancelled);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateConfirm3) {
        int ret;
        ret = conn->driver->domainMigrateConfirm3(domain,
                                                  cookiein, cookieinlen,
                                                  flags, cancelled);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
