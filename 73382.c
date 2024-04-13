virConnectDomainEventRegister(virConnectPtr conn,
                              virConnectDomainEventCallback cb,
                              void *opaque,
                              virFreeCallback freecb)
{
    VIR_DEBUG("conn=%p, cb=%p, opaque=%p, freecb=%p", conn, cb, opaque, freecb);
    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckNonNullArgGoto(cb, error);

    if (conn->driver && conn->driver->connectDomainEventRegister) {
        int ret;
        ret = conn->driver->connectDomainEventRegister(conn, cb, opaque, freecb);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
