virDomainSetMemoryParameters(virDomainPtr domain,
                             virTypedParameterPtr params,
                             int nparams, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%d, flags=%x",
                     params, nparams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(params, error);
    virCheckPositiveArgGoto(nparams, error);

    if (virTypedParameterValidateSet(conn, params, nparams) < 0)
        goto error;

    if (conn->driver->domainSetMemoryParameters) {
        int ret;
        ret = conn->driver->domainSetMemoryParameters(domain, params, nparams, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
