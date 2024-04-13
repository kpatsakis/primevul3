virDomainMigrate3(virDomainPtr domain,
                  virConnectPtr dconn,
                  virTypedParameterPtr params,
                  unsigned int nparams,
                  unsigned int flags)
{
    virDomainPtr ddomain = NULL;
    const char *compatParams[] = { VIR_MIGRATE_PARAM_URI,
                                   VIR_MIGRATE_PARAM_DEST_NAME,
                                   VIR_MIGRATE_PARAM_DEST_XML,
                                   VIR_MIGRATE_PARAM_BANDWIDTH };
    const char *uri = NULL;
    const char *dname = NULL;
    const char *dxml = NULL;
    unsigned long long bandwidth = 0;

    VIR_DOMAIN_DEBUG(domain, "dconn=%p, params=%p, nparms=%u flags=%x",
                     dconn, params, nparams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    /* First checkout the source */
    virCheckDomainReturn(domain, NULL);
    virCheckReadOnlyGoto(domain->conn->flags, error);

    /* Now checkout the destination */
    virCheckReadOnlyGoto(dconn->flags, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_MIGRATE_NON_SHARED_DISK,
                             VIR_MIGRATE_NON_SHARED_INC,
                             error);

    if (flags & VIR_MIGRATE_PEER2PEER) {
        virReportInvalidArg(flags, "%s",
                            _("use virDomainMigrateToURI3 for peer-to-peer "
                              "migration"));
        goto error;
    }
    if (flags & VIR_MIGRATE_TUNNELLED) {
        virReportInvalidArg(flags, "%s",
                            _("cannot perform tunnelled migration "
                              "without using peer2peer flag"));
        goto error;
    }

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

    /* Change protection requires support only on source side, and
     * is only needed in v3 migration, which automatically re-adds
     * the flag for just the source side.  We mask it out to allow
     * migration from newer source to an older destination that
     * rejects the flag.  */
    if (flags & VIR_MIGRATE_CHANGE_PROTECTION &&
        !VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                  VIR_DRV_FEATURE_MIGRATE_CHANGE_PROTECTION)) {
        virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                       _("cannot enforce change protection"));
        goto error;
    }
    flags &= ~VIR_MIGRATE_CHANGE_PROTECTION;

    /* Prefer extensible API but fall back to older migration APIs if params
     * only contains parameters which were supported by the older API. */
    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_MIGRATION_PARAMS) &&
        VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                 VIR_DRV_FEATURE_MIGRATION_PARAMS)) {
        VIR_DEBUG("Using migration protocol 3 with extensible parameters");
        ddomain = virDomainMigrateVersion3Params(domain, dconn, params,
                                                 nparams, flags);
        goto done;
    }

    if (!virTypedParamsCheck(params, nparams, compatParams,
                             ARRAY_CARDINALITY(compatParams))) {
        virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                       _("Migration APIs with extensible parameters are not "
                         "supported but extended parameters were passed"));
        goto error;
    }

    if (virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_URI, &uri) < 0 ||
        virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_DEST_NAME, &dname) < 0 ||
        virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_DEST_XML, &dxml) < 0 ||
        virTypedParamsGetULLong(params, nparams,
                                VIR_MIGRATE_PARAM_BANDWIDTH, &bandwidth) < 0) {
        goto error;
    }

    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_MIGRATION_V3) &&
        VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                 VIR_DRV_FEATURE_MIGRATION_V3)) {
        VIR_DEBUG("Using migration protocol 3");
        ddomain = virDomainMigrateVersion3(domain, dconn, dxml, flags,
                                           dname, uri, bandwidth);
    } else if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                        VIR_DRV_FEATURE_MIGRATION_V2) &&
               VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                      VIR_DRV_FEATURE_MIGRATION_V2)) {
        VIR_DEBUG("Using migration protocol 2");
        if (dxml) {
            virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                           _("Unable to change target guest XML during "
                             "migration"));
            goto error;
        }
        ddomain = virDomainMigrateVersion2(domain, dconn, flags,
                                           dname, uri, bandwidth);
    } else if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                        VIR_DRV_FEATURE_MIGRATION_V1) &&
               VIR_DRV_SUPPORTS_FEATURE(dconn->driver, dconn,
                                        VIR_DRV_FEATURE_MIGRATION_V1)) {
        VIR_DEBUG("Using migration protocol 1");
        if (dxml) {
            virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                           _("Unable to change target guest XML during "
                             "migration"));
            goto error;
        }
        ddomain = virDomainMigrateVersion1(domain, dconn, flags,
                                           dname, uri, bandwidth);
    } else {
        /* This driver does not support any migration method */
        virReportUnsupportedError();
        goto error;
    }

 done:
    if (ddomain == NULL)
        goto error;

    return ddomain;

 error:
    virDispatchError(domain->conn);
    return NULL;
}
