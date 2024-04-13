virDomainBlockCommit(virDomainPtr dom, const char *disk,
                     const char *base, const char *top,
                     unsigned long bandwidth, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, base=%s, top=%s, bandwidth=%lu, flags=%x",
                     disk, NULLSTR(base), NULLSTR(top), bandwidth, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(disk, error);

    if (conn->driver->domainBlockCommit) {
        int ret;
        ret = conn->driver->domainBlockCommit(dom, disk, base, top, bandwidth,
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
