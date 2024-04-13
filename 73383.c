virConnectDomainEventRegisterAny(virConnectPtr conn,
                                 virDomainPtr dom,
                                 int eventID,
                                 virConnectDomainEventGenericCallback cb,
                                 void *opaque,
                                 virFreeCallback freecb)
{
    VIR_DOMAIN_DEBUG(dom, "conn=%p, eventID=%d, cb=%p, opaque=%p, freecb=%p",
                     conn, eventID, cb, opaque, freecb);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    if (dom) {
        virCheckDomainGoto(dom, error);
        if (dom->conn != conn) {
            virReportInvalidArg(dom,
                                _("domain '%s' must match connection"),
                                dom->name);
            goto error;
        }
    }
    virCheckNonNullArgGoto(cb, error);
    virCheckNonNegativeArgGoto(eventID, error);
    if (eventID >= VIR_DOMAIN_EVENT_ID_LAST) {
        virReportInvalidArg(eventID,
                            _("eventID must be less than %d"),
                            VIR_DOMAIN_EVENT_ID_LAST);
        goto error;
    }

    if (conn->driver && conn->driver->connectDomainEventRegisterAny) {
        int ret;
        ret = conn->driver->connectDomainEventRegisterAny(conn, dom, eventID, cb, opaque, freecb);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
