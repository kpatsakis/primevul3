virConnectDomainXMLToNative(virConnectPtr conn,
                            const char *nativeFormat,
                            const char *domainXml,
                            unsigned int flags)
{
    VIR_DEBUG("conn=%p, format=%s, xml=%s, flags=%x",
              conn, NULLSTR(nativeFormat), NULLSTR(domainXml), flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckReadOnlyGoto(conn->flags, error);

    virCheckNonNullArgGoto(nativeFormat, error);
    virCheckNonNullArgGoto(domainXml, error);

    if (conn->driver->connectDomainXMLToNative) {
        char *ret;
        ret = conn->driver->connectDomainXMLToNative(conn,
                                                     nativeFormat,
                                                     domainXml,
                                                     flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
