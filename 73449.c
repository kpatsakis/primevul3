virDomainGetSecurityLabelList(virDomainPtr domain,
                              virSecurityLabelPtr* seclabels)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "seclabels=%p", seclabels);

    virResetLastError();

    virCheckDomainReturn(domain, -1);

    virCheckNonNullArgGoto(seclabels, error);

    conn = domain->conn;

    if (conn->driver->domainGetSecurityLabelList) {
        int ret;
        ret = conn->driver->domainGetSecurityLabelList(domain, seclabels);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
