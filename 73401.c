virDomainBlockPull(virDomainPtr dom, const char *disk,
                   unsigned long bandwidth, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, bandwidth=%lu, flags=%x",
                     disk, bandwidth, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(disk, error);

    if (conn->driver->domainBlockPull) {
        int ret;
        ret = conn->driver->domainBlockPull(dom, disk, bandwidth, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
