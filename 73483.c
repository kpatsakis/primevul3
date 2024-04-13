virDomainMigrateFinish3Params(virConnectPtr dconn,
                              virTypedParameterPtr params,
                              int nparams,
                              const char *cookiein,
                              int cookieinlen,
                              char **cookieout,
                              int *cookieoutlen,
                              unsigned int flags,
                              int cancelled)
{
    VIR_DEBUG("dconn=%p, params=%p, nparams=%d, cookiein=%p, cookieinlen=%d, "
              "cookieout=%p, cookieoutlen=%p, flags=%x, cancelled=%d",
              dconn, params, nparams, cookiein, cookieinlen, cookieout,
              cookieoutlen, flags, cancelled);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckConnectReturn(dconn, NULL);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigrateFinish3Params) {
        virDomainPtr ret;
        ret = dconn->driver->domainMigrateFinish3Params(
                dconn, params, nparams, cookiein, cookieinlen,
                cookieout, cookieoutlen, flags, cancelled);
        if (!ret && !cancelled)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return NULL;
}
