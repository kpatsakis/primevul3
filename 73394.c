virDomainAttachDevice(virDomainPtr domain, const char *xml)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "xml=%s", xml);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonNullArgGoto(xml, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainAttachDevice) {
       int ret;
       ret = conn->driver->domainAttachDevice(domain, xml);
       if (ret < 0)
          goto error;
       return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
