virConnectListAllDomains(virConnectPtr conn,
                         virDomainPtr **domains,
                         unsigned int flags)
{
    VIR_DEBUG("conn=%p, domains=%p, flags=%x", conn, domains, flags);

    virResetLastError();

    if (domains)
        *domains = NULL;

    virCheckConnectReturn(conn, -1);

    if (conn->driver->connectListAllDomains) {
        int ret;
        ret = conn->driver->connectListAllDomains(conn, domains, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
