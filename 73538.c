virDomainSetVcpus(virDomainPtr domain, unsigned int nvcpus)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "nvcpus=%u", nvcpus);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonZeroArgGoto(nvcpus, error);

    if (conn->driver->domainSetVcpus) {
        int ret;
        ret = conn->driver->domainSetVcpus(domain, nvcpus);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
