virDomainMigrateBegin3(virDomainPtr domain,
                       const char *xmlin,
                       char **cookieout,
                       int *cookieoutlen,
                       unsigned long flags,
                       const char *dname,
                       unsigned long bandwidth)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "xmlin=%s cookieout=%p, cookieoutlen=%p, "
                     "flags=%lx, dname=%s, bandwidth=%lu",
                     NULLSTR(xmlin), cookieout, cookieoutlen, flags,
                     NULLSTR(dname), bandwidth);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigrateBegin3) {
        char *xml;
        xml = conn->driver->domainMigrateBegin3(domain, xmlin,
                                                cookieout, cookieoutlen,
                                                flags, dname, bandwidth);
        VIR_DEBUG("xml %s", NULLSTR(xml));
        if (!xml)
            goto error;
        return xml;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
