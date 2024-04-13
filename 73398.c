virDomainBlockJobAbort(virDomainPtr dom, const char *disk,
                       unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, flags=%x", disk, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(disk, error);

    if (conn->driver->domainBlockJobAbort) {
        int ret;
        ret = conn->driver->domainBlockJobAbort(dom, disk, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
