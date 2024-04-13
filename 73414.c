virDomainDefineXML(virConnectPtr conn, const char *xml)
{
    VIR_DEBUG("conn=%p, xml=%s", conn, NULLSTR(xml));

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(xml, error);

    if (conn->driver->domainDefineXML) {
        virDomainPtr ret;
        ret = conn->driver->domainDefineXML(conn, xml);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
