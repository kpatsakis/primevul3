virDomainGetMetadata(virDomainPtr domain,
                     int type,
                     const char *uri,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "type=%d, uri='%s', flags=%x",
                     type, NULLSTR(uri), flags);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    switch (type) {
    case VIR_DOMAIN_METADATA_TITLE:
    case VIR_DOMAIN_METADATA_DESCRIPTION:
        virCheckNullArgGoto(uri, error);
        break;
    case VIR_DOMAIN_METADATA_ELEMENT:
        virCheckNonNullArgGoto(uri, error);
        break;
    default:
        /* For future expansion */
        break;
    }

    conn = domain->conn;

    if (conn->driver->domainGetMetadata) {
        char *ret;
        if (!(ret = conn->driver->domainGetMetadata(domain, type, uri, flags)))
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
