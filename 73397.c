virDomainBlockCopy(virDomainPtr dom, const char *disk,
                   const char *destxml,
                   virTypedParameterPtr params,
                   int nparams,
                   unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom,
                     "disk=%s, destxml=%s, params=%p, nparams=%d, flags=%x",
                     disk, destxml, params, nparams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(disk, error);
    virCheckNonNullArgGoto(destxml, error);
    virCheckNonNegativeArgGoto(nparams, error);
    if (nparams)
        virCheckNonNullArgGoto(params, error);

    if (conn->driver->domainBlockCopy) {
        int ret;
        ret = conn->driver->domainBlockCopy(dom, disk, destxml,
                                            params, nparams, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
