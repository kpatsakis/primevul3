virDomainGetXMLDesc(virDomainPtr domain, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    conn = domain->conn;

    if ((conn->flags & VIR_CONNECT_RO) &&
        (flags & (VIR_DOMAIN_XML_SECURE | VIR_DOMAIN_XML_MIGRATABLE))) {
        virReportError(VIR_ERR_OPERATION_DENIED, "%s",
                       _("virDomainGetXMLDesc with secure flag"));
        goto error;
    }

    if (conn->driver->domainGetXMLDesc) {
        char *ret;
        ret = conn->driver->domainGetXMLDesc(domain, flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
