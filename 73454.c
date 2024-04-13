virDomainGetVcpus(virDomainPtr domain, virVcpuInfoPtr info, int maxinfo,
                  unsigned char *cpumaps, int maplen)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "info=%p, maxinfo=%d, cpumaps=%p, maplen=%d",
                     info, maxinfo, cpumaps, maplen);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(info, error);
    virCheckPositiveArgGoto(maxinfo, error);

    /* Ensure that domainGetVcpus (aka remoteDomainGetVcpus) does not
       try to memcpy anything into a NULL pointer.  */
    if (cpumaps)
        virCheckPositiveArgGoto(maplen, error);
    else
        virCheckZeroArgGoto(maplen, error);

    if (cpumaps && INT_MULTIPLY_OVERFLOW(maxinfo, maplen)) {
        virReportError(VIR_ERR_OVERFLOW, _("input too large: %d * %d"),
                       maxinfo, maplen);
        goto error;
    }

    conn = domain->conn;

    if (conn->driver->domainGetVcpus) {
        int ret;
        ret = conn->driver->domainGetVcpus(domain, info, maxinfo,
                                           cpumaps, maplen);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
