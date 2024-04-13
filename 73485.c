virDomainMigratePerform(virDomainPtr domain,
                        const char *cookie,
                        int cookielen,
                        const char *uri,
                        unsigned long flags,
                        const char *dname,
                        unsigned long bandwidth)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "cookie=%p, cookielen=%d, uri=%s, flags=%lx, "
                     "dname=%s, bandwidth=%lu", cookie, cookielen, uri, flags,
                     NULLSTR(dname), bandwidth);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigratePerform) {
        int ret;
        ret = conn->driver->domainMigratePerform(domain, cookie, cookielen,
                                                 uri,
                                                 flags, dname, bandwidth);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
