virDomainMigrateToURI3(virDomainPtr domain,
                       const char *dconnuri,
                       virTypedParameterPtr params,
                       unsigned int nparams,
                       unsigned int flags)
{
    VIR_DOMAIN_DEBUG(domain, "dconnuri=%s, params=%p, nparms=%u flags=%x",
                     NULLSTR(dconnuri), params, nparams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

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

    if (virDomainMigrateUnmanagedParams(domain, dconnuri,
                                        params, nparams, flags) < 0)
        goto error;

    return 0;

 error:
    virDispatchError(domain->conn);
    return -1;
}
