virDomainGetVcpuPinInfo(virDomainPtr domain, int ncpumaps,
                        unsigned char *cpumaps, int maplen, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "ncpumaps=%d, cpumaps=%p, maplen=%d, flags=%x",
                     ncpumaps, cpumaps, maplen, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonNullArgGoto(cpumaps, error);
    virCheckPositiveArgGoto(ncpumaps, error);
    virCheckPositiveArgGoto(maplen, error);

    if (INT_MULTIPLY_OVERFLOW(ncpumaps, maplen)) {
        virReportError(VIR_ERR_OVERFLOW, _("input too large: %d * %d"),
                       ncpumaps, maplen);
        goto error;
    }

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    if (conn->driver->domainGetVcpuPinInfo) {
        int ret;
        ret = conn->driver->domainGetVcpuPinInfo(domain, ncpumaps,
                                                 cpumaps, maplen, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
