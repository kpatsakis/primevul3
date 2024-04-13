virDomainGetSchedulerParametersFlags(virDomainPtr domain,
                                     virTypedParameterPtr params, int *nparams,
                                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%p, flags=%x",
                     params, nparams, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);

    virCheckNonNullArgGoto(params, error);
    virCheckNonNullArgGoto(nparams, error);
    virCheckPositiveArgGoto(*nparams, error);

    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_TYPED_PARAM_STRING))
        flags |= VIR_TYPED_PARAM_STRING_OKAY;

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    conn = domain->conn;

    if (conn->driver->domainGetSchedulerParametersFlags) {
        int ret;
        ret = conn->driver->domainGetSchedulerParametersFlags(domain, params,
                                                              nparams, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
