virDomainDetachDeviceFlags(virDomainPtr domain,
                           const char *xml, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "xml=%s, flags=%x", xml, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonNullArgGoto(xml, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainDetachDeviceFlags) {
        int ret;
        ret = conn->driver->domainDetachDeviceFlags(domain, xml, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
