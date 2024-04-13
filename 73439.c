virDomainGetJobStats(virDomainPtr domain,
                     int *type,
                     virTypedParameterPtr *params,
                     int *nparams,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "type=%p, params=%p, nparams=%p, flags=%x",
                     type, params, nparams, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(type, error);
    virCheckNonNullArgGoto(params, error);
    virCheckNonNullArgGoto(nparams, error);

    conn = domain->conn;

    if (conn->driver->domainGetJobStats) {
        int ret;
        ret = conn->driver->domainGetJobStats(domain, type, params,
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
