virDomainGetBlockIoTune(virDomainPtr dom,
                        const char *disk,
                        virTypedParameterPtr params,
                        int *nparams,
                        unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, params=%p, nparams=%d, flags=%x",
                     NULLSTR(disk), params, (nparams) ? *nparams : -1, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);

    virCheckNonNullArgGoto(nparams, error);
    virCheckNonNegativeArgGoto(*nparams, error);
    if (*nparams != 0) {
        virCheckNonNullArgGoto(params, error);
        virCheckNonNullArgGoto(disk, error);
    }

    if (VIR_DRV_SUPPORTS_FEATURE(dom->conn->driver, dom->conn,
                                 VIR_DRV_FEATURE_TYPED_PARAM_STRING))
        flags |= VIR_TYPED_PARAM_STRING_OKAY;

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    conn = dom->conn;

    if (conn->driver->domainGetBlockIoTune) {
        int ret;
        ret = conn->driver->domainGetBlockIoTune(dom, disk, params, nparams, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
