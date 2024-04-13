virDomainMigrateUnmanagedProto3(virDomainPtr domain,
                                const char *dconnuri,
                                virTypedParameterPtr params,
                                int nparams,
                                unsigned int flags)
{
    const char *compatParams[] = { VIR_MIGRATE_PARAM_URI,
                                   VIR_MIGRATE_PARAM_DEST_NAME,
                                   VIR_MIGRATE_PARAM_DEST_XML,
                                   VIR_MIGRATE_PARAM_BANDWIDTH };
    const char *miguri = NULL;
    const char *dname = NULL;
    const char *xmlin = NULL;
    unsigned long long bandwidth = 0;

    if (!virTypedParamsCheck(params, nparams, compatParams,
                             ARRAY_CARDINALITY(compatParams))) {
        virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                       _("Some parameters are not supported by migration "
                         "protocol 3"));
        return -1;
    }

    if (virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_URI, &miguri) < 0 ||
        virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_DEST_NAME, &dname) < 0 ||
        virTypedParamsGetString(params, nparams,
                                VIR_MIGRATE_PARAM_DEST_XML, &xmlin) < 0 ||
        virTypedParamsGetULLong(params, nparams,
                                VIR_MIGRATE_PARAM_BANDWIDTH, &bandwidth) < 0) {
        return -1;
    }

    return domain->conn->driver->domainMigratePerform3
            (domain, xmlin, NULL, 0, NULL, NULL, dconnuri,
             miguri, flags, dname, bandwidth);
}
