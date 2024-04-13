virDomainGetMaxMemory(virDomainPtr domain)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain);

    virResetLastError();

    virCheckDomainReturn(domain, 0);
    conn = domain->conn;

    if (conn->driver->domainGetMaxMemory) {
        unsigned long long ret;
        ret = conn->driver->domainGetMaxMemory(domain);
        if (ret == 0)
            goto error;
        if ((unsigned long) ret != ret) {
            virReportError(VIR_ERR_OVERFLOW, _("result too large: %llu"),
                           ret);
            goto error;
        }
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return 0;
}
