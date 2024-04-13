virDomainSendProcessSignal(virDomainPtr domain,
                           long long pid_value,
                           unsigned int signum,
                           unsigned int flags)
{
    virConnectPtr conn;
    VIR_DOMAIN_DEBUG(domain, "pid=%lld, signum=%u flags=%x",
                     pid_value, signum, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonZeroArgGoto(pid_value, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainSendProcessSignal) {
        int ret;
        ret = conn->driver->domainSendProcessSignal(domain,
                                                    pid_value,
                                                    signum,
                                                    flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
