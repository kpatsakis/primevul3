virDomainBlockResize(virDomainPtr dom,
                     const char *disk,
                     unsigned long long size,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, size=%llu, flags=%x", disk, size, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(disk, error);

    if (conn->driver->domainBlockResize) {
        int ret;
        ret = conn->driver->domainBlockResize(dom, disk, size, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
