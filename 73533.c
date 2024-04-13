virDomainSetMetadata(virDomainPtr domain,
                     int type,
                     const char *metadata,
                     const char *key,
                     const char *uri,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain,
                     "type=%d, metadata='%s', key='%s', uri='%s', flags=%x",
                     type, NULLSTR(metadata), NULLSTR(key), NULLSTR(uri),
                     flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    switch (type) {
    case VIR_DOMAIN_METADATA_TITLE:
        if (metadata && strchr(metadata, '\n')) {
            virReportInvalidArg(metadata, "%s",
                                _("metadata title can't contain "
                                  "newlines"));
            goto error;
        }
        /* fallthrough */
    case VIR_DOMAIN_METADATA_DESCRIPTION:
        virCheckNullArgGoto(uri, error);
        virCheckNullArgGoto(key, error);
        break;
    case VIR_DOMAIN_METADATA_ELEMENT:
        virCheckNonNullArgGoto(uri, error);
        if (metadata)
            virCheckNonNullArgGoto(key, error);
        break;
    default:
        /* For future expansion */
        break;
    }

    if (conn->driver->domainSetMetadata) {
        int ret;
        ret = conn->driver->domainSetMetadata(domain, type, metadata, key, uri,
                                              flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
