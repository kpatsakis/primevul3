virDomainMigrateUnmanaged(virDomainPtr domain,
                          const char *xmlin,
                          unsigned int flags,
                          const char *dname,
                          const char *dconnuri,
                          const char *miguri,
                          unsigned long long bandwidth)
{
    int ret = -1;
    virTypedParameterPtr params = NULL;
    int nparams = 0;
    int maxparams = 0;

    if (miguri &&
        virTypedParamsAddString(&params, &nparams, &maxparams,
                                VIR_MIGRATE_PARAM_URI, miguri) < 0)
        goto cleanup;
    if (dname &&
        virTypedParamsAddString(&params, &nparams, &maxparams,
                                VIR_MIGRATE_PARAM_DEST_NAME, dname) < 0)
        goto cleanup;
    if (xmlin &&
        virTypedParamsAddString(&params, &nparams, &maxparams,
                                VIR_MIGRATE_PARAM_DEST_XML, xmlin) < 0)
        goto cleanup;
    if (virTypedParamsAddULLong(&params, &nparams, &maxparams,
                                VIR_MIGRATE_PARAM_BANDWIDTH, bandwidth) < 0)
        goto cleanup;

    ret = virDomainMigrateUnmanagedParams(domain, dconnuri, params,
                                          nparams, flags);

 cleanup:
    virTypedParamsFree(params, nparams);

    return ret;
}
