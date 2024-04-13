virDomainMigrate(virDomainPtr domain,
                 virConnectPtr dconn,
                 unsigned long flags,
                 const char *dname,
                 const char *uri,
                 unsigned long bandwidth)
{
    virDomainPtr ddomain = NULL;

    VIR_DOMAIN_DEBUG(domain,
                     "dconn=%p, flags=%lx, dname=%s, uri=%s, bandwidth=%lu",
                     dconn, flags, NULLSTR(dname), NULLSTR(uri), bandwidth);

    virResetLastError();

    /* First checkout the source */
    virCheckDomainReturn(domain, NULL);
    virCheckReadOnlyGoto(domain->conn->flags, error);

    /* Now checkout the destination */
    virCheckConnectGoto(dconn, error);
    virCheckReadOnlyGoto(dconn->flags, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_MIGRATE_NON_SHARED_DISK,
                             VIR_MIGRATE_NON_SHARED_INC,
                             error);

    if (flags & VIR_MIGRATE_OFFLINE) {
        if (!VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                      VIR_DRV_FEATURE_MIGRATION_OFFLINE)) {
            virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                           _("offline migration is not supported by "
                             "the source host"));
            goto error;
        }
        if (!VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                      VIR_DRV_FEATURE_MIGRATION_OFFLINE)) {
            virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                           _("offline migration is not supported by "
                             "the destination host"));
            goto error;
        }
    }

    if (flags & VIR_MIGRATE_PEER2PEER) {
        if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                     VIR_DRV_FEATURE_MIGRATION_P2P)) {
            char *dstURI = NULL;
            if (uri == NULL) {
                dstURI = virConnectGetURI(dconn);
                if (!dstURI)
                    return NULL;
            }

            VIR_DEBUG("Using peer2peer migration");
            if (virDomainMigrateUnmanaged(domain, NULL, flags, dname,
                                          uri ? uri : dstURI, NULL, bandwidth) < 0) {
                VIR_FREE(dstURI);
                goto error;
            }
            VIR_FREE(dstURI);

            ddomain = virDomainLookupByName(dconn, dname ? dname : domain->name);
        } else {
            /* This driver does not support peer to peer migration */
            virReportUnsupportedError();
            goto error;
        }
    } else {
        /* Change protection requires support only on source side, and
         * is only needed in v3 migration, which automatically re-adds
         * the flag for just the source side.  We mask it out for
         * non-peer2peer to allow migration from newer source to an
         * older destination that rejects the flag.  */
        if (flags & VIR_MIGRATE_CHANGE_PROTECTION &&
            !VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                      VIR_DRV_FEATURE_MIGRATE_CHANGE_PROTECTION)) {
            virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                           _("cannot enforce change protection"));
            goto error;
        }
        flags &= ~VIR_MIGRATE_CHANGE_PROTECTION;
        if (flags & VIR_MIGRATE_TUNNELLED) {
            virReportError(VIR_ERR_OPERATION_INVALID, "%s",
                           _("cannot perform tunnelled migration without using peer2peer flag"));
            goto error;
        }

        /* Check that migration is supported by both drivers. */
        if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                     VIR_DRV_FEATURE_MIGRATION_V3) &&
            VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                     VIR_DRV_FEATURE_MIGRATION_V3)) {
            VIR_DEBUG("Using migration protocol 3");
            ddomain = virDomainMigrateVersion3(domain, dconn, NULL,
                                               flags, dname, uri, bandwidth);
        } else if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                            VIR_DRV_FEATURE_MIGRATION_V2) &&
                   VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                          VIR_DRV_FEATURE_MIGRATION_V2)) {
            VIR_DEBUG("Using migration protocol 2");
            ddomain = virDomainMigrateVersion2(domain, dconn, flags,
                                               dname, uri, bandwidth);
        } else if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                            VIR_DRV_FEATURE_MIGRATION_V1) &&
                   VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                            VIR_DRV_FEATURE_MIGRATION_V1)) {
            VIR_DEBUG("Using migration protocol 1");
            ddomain = virDomainMigrateVersion1(domain, dconn, flags,
                                               dname, uri, bandwidth);
        } else {
            /* This driver does not support any migration method */
            virReportUnsupportedError();
            goto error;
        }
    }

    if (ddomain == NULL)
        goto error;

    return ddomain;

 error:
    virDispatchError(domain->conn);
    return NULL;
}
