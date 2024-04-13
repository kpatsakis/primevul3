virDomainGetSecurityLabel(virDomainPtr domain, virSecurityLabelPtr seclabel)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "seclabel=%p", seclabel);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonNullArgGoto(seclabel, error);

    if (conn->driver->domainGetSecurityLabel) {
        int ret;
        ret = conn->driver->domainGetSecurityLabel(domain, seclabel);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
