virDomainSetInterfaceParameters(virDomainPtr domain,
                                const char *device,
                                virTypedParameterPtr params,
                                int nparams, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "device=%s, params=%p, nparams=%d, flags=%x",
                     device, params, nparams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(params, error);
    virCheckPositiveArgGoto(nparams, error);

    if (virTypedParameterValidateSet(conn, params, nparams) < 0)
        goto error;

    if (conn->driver->domainSetInterfaceParameters) {
        int ret;
        ret = conn->driver->domainSetInterfaceParameters(domain, device,
                                                         params, nparams,
                                                         flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
