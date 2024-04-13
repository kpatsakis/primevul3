virDomainGetVcpusFlags(virDomainPtr domain, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    if (flags & VIR_DOMAIN_VCPU_GUEST)
        virCheckReadOnlyGoto(conn->flags, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    if (conn->driver->domainGetVcpusFlags) {
        int ret;
        ret = conn->driver->domainGetVcpusFlags(domain, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
