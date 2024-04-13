virDomainGetIOThreadInfo(virDomainPtr dom,
                         virDomainIOThreadInfoPtr **info,
                         unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "info=%p flags=%x", info, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckNonNullArgGoto(info, error);
    *info = NULL;

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    if (dom->conn->driver->domainGetIOThreadInfo) {
        int ret;
        ret = dom->conn->driver->domainGetIOThreadInfo(dom, info, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
