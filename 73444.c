virDomainGetOSType(virDomainPtr domain)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    conn = domain->conn;

    if (conn->driver->domainGetOSType) {
        char *ret;
        ret = conn->driver->domainGetOSType(domain);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
