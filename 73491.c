virDomainMigrateSetCompressionCache(virDomainPtr domain,
                                    unsigned long long cacheSize,
                                    unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "cacheSize=%llu, flags=%x", cacheSize, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateSetCompressionCache) {
        if (conn->driver->domainMigrateSetCompressionCache(domain, cacheSize,
                                                           flags) < 0)
            goto error;
        return 0;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
