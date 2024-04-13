virDomainDefineXMLFlags(virConnectPtr conn, const char *xml, unsigned int flags)
{
    VIR_DEBUG("conn=%p, xml=%s flags=%x", conn, NULLSTR(xml), flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(xml, error);

    if (conn->driver->domainDefineXMLFlags) {
        virDomainPtr ret;
        ret = conn->driver->domainDefineXMLFlags(conn, xml, flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
