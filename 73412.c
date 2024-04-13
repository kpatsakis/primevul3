virDomainCreateXML(virConnectPtr conn, const char *xmlDesc,
                   unsigned int flags)
{
    VIR_DEBUG("conn=%p, xmlDesc=%s, flags=%x", conn, NULLSTR(xmlDesc), flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNullArgGoto(xmlDesc, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainCreateXML) {
        virDomainPtr ret;
        ret = conn->driver->domainCreateXML(conn, xmlDesc, flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return NULL;
}
