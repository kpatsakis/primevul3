virConnectDomainXMLFromNative(virConnectPtr conn,
                              const char *nativeFormat,
                              const char *nativeConfig,
                              unsigned int flags)
{
    VIR_DEBUG("conn=%p, format=%s, config=%s, flags=%x",
              conn, NULLSTR(nativeFormat), NULLSTR(nativeConfig), flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckReadOnlyGoto(conn->flags, error);

    virCheckNonNullArgGoto(nativeFormat, error);
    virCheckNonNullArgGoto(nativeConfig, error);

    if (conn->driver->connectDomainXMLFromNative) {
        char *ret;
        ret = conn->driver->connectDomainXMLFromNative(conn,
                                                       nativeFormat,
                                                       nativeConfig,
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
