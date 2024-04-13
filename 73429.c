virDomainGetBlockJobInfo(virDomainPtr dom, const char *disk,
                         virDomainBlockJobInfoPtr info, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, info=%p, flags=%x", disk, info, flags);

    virResetLastError();

    if (info)
        memset(info, 0, sizeof(*info));

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckNonNullArgGoto(disk, error);
    virCheckNonNullArgGoto(info, error);

    if (conn->driver->domainGetBlockJobInfo) {
        int ret;
        ret = conn->driver->domainGetBlockJobInfo(dom, disk, info, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
