virConnectNumOfDefinedDomains(virConnectPtr conn)
{
    VIR_DEBUG("conn=%p", conn);

    virResetLastError();

    virCheckConnectReturn(conn, -1);

    if (conn->driver->connectNumOfDefinedDomains) {
        int ret;
        ret = conn->driver->connectNumOfDefinedDomains(conn);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
