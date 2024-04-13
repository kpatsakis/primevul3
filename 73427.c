virDomainGetBlkioParameters(virDomainPtr domain,
                            virTypedParameterPtr params,
                            int *nparams, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%d, flags=%x",
                     params, (nparams) ? *nparams : -1, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(nparams, error);
    virCheckNonNegativeArgGoto(*nparams, error);
    if (*nparams != 0)
        virCheckNonNullArgGoto(params, error);

    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_TYPED_PARAM_STRING))
        flags |= VIR_TYPED_PARAM_STRING_OKAY;

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    conn = domain->conn;

    if (conn->driver->domainGetBlkioParameters) {
        int ret;
        ret = conn->driver->domainGetBlkioParameters(domain, params, nparams, flags);
        if (ret < 0)
            goto error;
        return ret;
    }
    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
