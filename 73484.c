virDomainMigrateGetMaxSpeed(virDomainPtr domain,
                            unsigned long *bandwidth,
                            unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "bandwidth = %p, flags=%x", bandwidth, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonNullArgGoto(bandwidth, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateGetMaxSpeed) {
        if (conn->driver->domainMigrateGetMaxSpeed(domain, bandwidth, flags) < 0)
            goto error;
        return 0;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
