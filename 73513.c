virDomainPinVcpu(virDomainPtr domain, unsigned int vcpu,
                 unsigned char *cpumap, int maplen)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "vcpu=%u, cpumap=%p, maplen=%d",
                     vcpu, cpumap, maplen);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(cpumap, error);
    virCheckPositiveArgGoto(maplen, error);

    if (conn->driver->domainPinVcpu) {
        int ret;
        ret = conn->driver->domainPinVcpu(domain, vcpu, cpumap, maplen);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
