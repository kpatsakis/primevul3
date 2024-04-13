virDomainSetSchedulerParameters(virDomainPtr domain,
                                virTypedParameterPtr params, int nparams)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%d", params, nparams);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(params, error);
    virCheckNonNegativeArgGoto(nparams, error);

    if (virTypedParameterValidateSet(conn, params, nparams) < 0)
        goto error;

    if (conn->driver->domainSetSchedulerParameters) {
        int ret;
        ret = conn->driver->domainSetSchedulerParameters(domain, params, nparams);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
