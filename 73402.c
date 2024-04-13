virDomainBlockRebase(virDomainPtr dom, const char *disk,
                     const char *base, unsigned long bandwidth,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, base=%s, bandwidth=%lu, flags=%x",
                     disk, NULLSTR(base), bandwidth, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(disk, error);

    if (flags & VIR_DOMAIN_BLOCK_REBASE_COPY) {
        virCheckNonNullArgGoto(base, error);
    } else if (flags & (VIR_DOMAIN_BLOCK_REBASE_SHALLOW |
                        VIR_DOMAIN_BLOCK_REBASE_REUSE_EXT |
                        VIR_DOMAIN_BLOCK_REBASE_COPY_RAW |
                        VIR_DOMAIN_BLOCK_REBASE_COPY_DEV)) {
        virReportInvalidArg(flags, "%s",
                            _("use of flags requires a copy job"));
        goto error;
    }

    if (conn->driver->domainBlockRebase) {
        int ret;
        ret = conn->driver->domainBlockRebase(dom, disk, base, bandwidth,
                                              flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
