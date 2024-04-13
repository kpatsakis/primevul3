virDomainMigrateConfirm3Params(virDomainPtr domain,
                               virTypedParameterPtr params,
                               int nparams,
                               const char *cookiein,
                               int cookieinlen,
                               unsigned int flags,
                               int cancelled)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%d, cookiein=%p, "
                     "cookieinlen=%d, flags=%x, cancelled=%d",
                     params, nparams, cookiein, cookieinlen, flags, cancelled);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateConfirm3Params) {
        int ret;
        ret = conn->driver->domainMigrateConfirm3Params(
                domain, params, nparams,
                cookiein, cookieinlen, flags, cancelled);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
