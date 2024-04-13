virDomainMigrateSetMaxDowntime(virDomainPtr domain,
                               unsigned long long downtime,
                               unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "downtime=%llu, flags=%x", downtime, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateSetMaxDowntime) {
        if (conn->driver->domainMigrateSetMaxDowntime(domain, downtime, flags) < 0)
            goto error;
        return 0;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
