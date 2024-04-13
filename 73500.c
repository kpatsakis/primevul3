virDomainMigrateUnmanagedProto2(virDomainPtr domain,
                                const char *dconnuri,
                                virTypedParameterPtr params,
                                int nparams,
                                unsigned int flags)
{
    /* uri parameter is added for direct case */
    const char *compatParams[] = { VIR_MIGRATE_PARAM_DEST_NAME,
                                   VIR_MIGRATE_PARAM_BANDWIDTH,
                                   VIR_MIGRATE_PARAM_URI };
    const char *uri = NULL;
    const char *miguri = NULL;
    const char *dname = NULL;
    unsigned long long bandwidth = 0;

    if (!virTypedParamsCheck(params, nparams, compatParams,
                             ARRAY_CARDINALITY(compatParams))) {
        virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                       _("Some parameters are not supported by migration "
                         "protocol 2"));
        return -1;
    }

    if (virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_URI, &miguri) < 0 ||
        virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_DEST_NAME, &dname) < 0 ||
        virTypedParamsGetULLong(params, nparams,
                                VIR_MIGRATE_PARAM_BANDWIDTH, &bandwidth) < 0) {
        return -1;
    }

    if (flags & VIR_MIGRATE_PEER2PEER) {
        if (miguri) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("Unable to override peer2peer migration URI"));
            return -1;
        }
        uri = dconnuri;
    } else {
        uri = miguri;
    }

    return domain->conn->driver->domainMigratePerform
            (domain, NULL, 0, uri, flags, dname, bandwidth);
}
