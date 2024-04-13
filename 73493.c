virDomainMigrateSetMaxSpeed(virDomainPtr domain,
                            unsigned long bandwidth,
                            unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "bandwidth=%lu, flags=%x", bandwidth, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateSetMaxSpeed) {
        if (conn->driver->domainMigrateSetMaxSpeed(domain, bandwidth, flags) < 0)
            goto error;
        return 0;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
