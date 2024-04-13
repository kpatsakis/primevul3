virDomainMigratePrepareTunnel3Params(virConnectPtr conn,
                                     virStreamPtr st,
                                     virTypedParameterPtr params,
                                     int nparams,
                                     const char *cookiein,
                                     int cookieinlen,
                                     char **cookieout,
                                     int *cookieoutlen,
                                     unsigned int flags)
{
    VIR_DEBUG("conn=%p, stream=%p, params=%p, nparams=%d, cookiein=%p, "
              "cookieinlen=%d, cookieout=%p, cookieoutlen=%p, flags=%x",
              conn, st, params, nparams, cookiein, cookieinlen,
              cookieout, cookieoutlen, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn != st->conn) {
        virReportInvalidArg(conn, "%s",
                            _("conn must match stream connection"));
        goto error;
    }

    if (conn->driver->domainMigratePrepareTunnel3Params) {
        int rv;
        rv = conn->driver->domainMigratePrepareTunnel3Params(
                conn, st, params, nparams, cookiein, cookieinlen,
                cookieout, cookieoutlen, flags);
        if (rv < 0)
            goto error;
        return rv;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
