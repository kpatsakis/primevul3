virDomainMigratePrepare3Params(virConnectPtr dconn,
                               virTypedParameterPtr params,
                               int nparams,
                               const char *cookiein,
                               int cookieinlen,
                               char **cookieout,
                               int *cookieoutlen,
                               char **uri_out,
                               unsigned int flags)
{
    VIR_DEBUG("dconn=%p, params=%p, nparams=%d, cookiein=%p, cookieinlen=%d, "
              "cookieout=%p, cookieoutlen=%p, uri_out=%p, flags=%x",
              dconn, params, nparams, cookiein, cookieinlen,
              cookieout, cookieoutlen, uri_out, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckConnectReturn(dconn, -1);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigratePrepare3Params) {
        int ret;
        ret = dconn->driver->domainMigratePrepare3Params(dconn, params, nparams,
                                                         cookiein, cookieinlen,
                                                         cookieout, cookieoutlen,
                                                         uri_out, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return -1;
}
