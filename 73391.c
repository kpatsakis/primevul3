virConnectNumOfDomains(virConnectPtr conn)
{
    VIR_DEBUG("conn=%p", conn);

    virResetLastError();

    virCheckConnectReturn(conn, -1);

    if (conn->driver->connectNumOfDomains) {
        int ret = conn->driver->connectNumOfDomains(conn);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
