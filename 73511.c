virDomainPinEmulator(virDomainPtr domain, unsigned char *cpumap,
                     int maplen, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "cpumap=%p, maplen=%d, flags=%x",
                     cpumap, maplen, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    virCheckNonNullArgGoto(cpumap, error);
    virCheckPositiveArgGoto(maplen, error);

    if (conn->driver->domainPinEmulator) {
        int ret;
        ret = conn->driver->domainPinEmulator(domain, cpumap, maplen, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
