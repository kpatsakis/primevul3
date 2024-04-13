virDomainSetVcpusFlags(virDomainPtr domain, unsigned int nvcpus,
                       unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "nvcpus=%u, flags=%x", nvcpus, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckReadOnlyGoto(domain->conn->flags, error);

    VIR_REQUIRE_FLAG_GOTO(VIR_DOMAIN_VCPU_MAXIMUM,
                          VIR_DOMAIN_AFFECT_CONFIG,
                          error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_VCPU_GUEST,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    virCheckNonZeroArgGoto(nvcpus, error);

    conn = domain->conn;

    if (conn->driver->domainSetVcpusFlags) {
        int ret;
        ret = conn->driver->domainSetVcpusFlags(domain, nvcpus, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
