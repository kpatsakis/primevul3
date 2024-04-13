virDomainDelIOThread(virDomainPtr domain,
                     unsigned int iothread_id,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "iothread_id=%u, flags=%x", iothread_id, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckReadOnlyGoto(domain->conn->flags, error);
    virCheckNonZeroArgGoto(iothread_id, error);

    conn = domain->conn;

    if (conn->driver->domainDelIOThread) {
        int ret;
        ret = conn->driver->domainDelIOThread(domain, iothread_id, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
