virDomainGetSchedulerParameters(virDomainPtr domain,
                                virTypedParameterPtr params, int *nparams)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%p", params, nparams);

    virResetLastError();

    virCheckDomainReturn(domain, -1);

    virCheckNonNullArgGoto(params, error);
    virCheckNonNullArgGoto(nparams, error);
    virCheckPositiveArgGoto(*nparams, error);

    conn = domain->conn;

    if (conn->driver->domainGetSchedulerParameters) {
        int ret;
        ret = conn->driver->domainGetSchedulerParameters(domain, params, nparams);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
