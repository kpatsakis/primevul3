virConnectDomainEventDeregisterAny(virConnectPtr conn,
                                   int callbackID)
{
    VIR_DEBUG("conn=%p, callbackID=%d", conn, callbackID);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckNonNegativeArgGoto(callbackID, error);

    if (conn->driver && conn->driver->connectDomainEventDeregisterAny) {
        int ret;
        ret = conn->driver->connectDomainEventDeregisterAny(conn, callbackID);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
