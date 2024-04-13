virDomainMigrateToURI(virDomainPtr domain,
                      const char *duri,
                      unsigned long flags,
                      const char *dname,
                      unsigned long bandwidth)
{
    const char *dconnuri = NULL;
    const char *miguri = NULL;

    VIR_DOMAIN_DEBUG(domain, "duri=%p, flags=%lx, dname=%s, bandwidth=%lu",
                     NULLSTR(duri), flags, NULLSTR(dname), bandwidth);

    virResetLastError();

    /* First checkout the source */
    virCheckDomainReturn(domain, -1);
    virCheckReadOnlyGoto(domain->conn->flags, error);
    virCheckNonNullArgGoto(duri, error);

    if (virDomainMigrateUnmanagedCheckCompat(domain, flags) < 0)
        goto error;

    if (flags & VIR_MIGRATE_PEER2PEER)
        dconnuri = duri;
    else
        miguri = duri;

    if (virDomainMigrateUnmanaged(domain, NULL, flags,
                                  dname, dconnuri, miguri, bandwidth) < 0)
        goto error;

    return 0;

 error:
    virDispatchError(domain->conn);
    return -1;
}
