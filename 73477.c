virDomainMigrateBegin3Params(virDomainPtr domain,
                             virTypedParameterPtr params,
                             int nparams,
                             char **cookieout,
                             int *cookieoutlen,
                             unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "params=%p, nparams=%d, "
                     "cookieout=%p, cookieoutlen=%p, flags=%x",
                     params, nparams, cookieout, cookieoutlen, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateBegin3Params) {
        char *xml;
        xml = conn->driver->domainMigrateBegin3Params(domain, params, nparams,
                                                      cookieout, cookieoutlen,
                                                      flags);
        VIR_DEBUG("xml %s", NULLSTR(xml));
        if (!xml)
            goto error;
        return xml;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
