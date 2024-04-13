virDomainMigrateToURI2(virDomainPtr domain,
                       const char *dconnuri,
                       const char *miguri,
                       const char *dxml,
                       unsigned long flags,
                       const char *dname,
                       unsigned long bandwidth)
{
    VIR_DOMAIN_DEBUG(domain, "dconnuri=%s, miguri=%s, dxml=%s, "
                     "flags=%lx, dname=%s, bandwidth=%lu",
                     NULLSTR(dconnuri), NULLSTR(miguri), NULLSTR(dxml),
                     flags, NULLSTR(dname), bandwidth);

    virResetLastError();

    /* First checkout the source */
    virCheckDomainReturn(domain, -1);
    virCheckReadOnlyGoto(domain->conn->flags, error);

    if (virDomainMigrateUnmanagedCheckCompat(domain, flags) < 0)
        goto error;

    if (flags & VIR_MIGRATE_PEER2PEER)
        virCheckNonNullArgGoto(dconnuri, error);
    else
        dconnuri = NULL;

    if (virDomainMigrateUnmanaged(domain, dxml, flags,
                                  dname, dconnuri, miguri, bandwidth) < 0)
        goto error;

    return 0;

 error:
    virDispatchError(domain->conn);
    return -1;
}
