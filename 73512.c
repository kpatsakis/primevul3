virDomainPinIOThread(virDomainPtr domain,
                     unsigned int iothread_id,
                     unsigned char *cpumap,
                     int maplen,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "iothread_id=%u, cpumap=%p, maplen=%d",
                     iothread_id, cpumap, maplen);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(cpumap, error);
    virCheckPositiveArgGoto(maplen, error);

    if (conn->driver->domainPinIOThread) {
        int ret;
        ret = conn->driver->domainPinIOThread(domain, iothread_id,
                                              cpumap, maplen, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
