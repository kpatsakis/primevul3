virDomainGetFSInfo(virDomainPtr dom,
                   virDomainFSInfoPtr **info,
                   unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "info=%p, flags=%x", info, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckReadOnlyGoto(dom->conn->flags, error);
    virCheckNonNullArgGoto(info, error);
    *info = NULL;

    if (dom->conn->driver->domainGetFSInfo) {
        int ret = dom->conn->driver->domainGetFSInfo(dom, info, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
