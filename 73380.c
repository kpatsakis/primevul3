virConnectDomainEventDeregister(virConnectPtr conn,
                                virConnectDomainEventCallback cb)
{
    VIR_DEBUG("conn=%p, cb=%p", conn, cb);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckNonNullArgGoto(cb, error);

    if (conn->driver && conn->driver->connectDomainEventDeregister) {
        int ret;
        ret = conn->driver->connectDomainEventDeregister(conn, cb);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
