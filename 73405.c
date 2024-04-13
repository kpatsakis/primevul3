virDomainBlockStatsFlags(virDomainPtr dom,
                         const char *disk,
                         virTypedParameterPtr params,
                         int *nparams,
                         unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, params=%p, nparams=%d, flags=%x",
                     disk, params, nparams ? *nparams : -1, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckNonNullArgGoto(disk, error);
    virCheckNonNullArgGoto(nparams, error);
    virCheckNonNegativeArgGoto(*nparams, error);
    if (*nparams != 0)
        virCheckNonNullArgGoto(params, error);

    if (VIR_DRV_SUPPORTS_FEATURE(dom->conn->driver, dom->conn,
                                 VIR_DRV_FEATURE_TYPED_PARAM_STRING))
        flags |= VIR_TYPED_PARAM_STRING_OKAY;
    conn = dom->conn;

    if (conn->driver->domainBlockStatsFlags) {
        int ret;
        ret = conn->driver->domainBlockStatsFlags(dom, disk, params, nparams, flags);
        if (ret < 0)
            goto error;
        return ret;
    }
    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
