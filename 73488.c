virDomainMigratePrepareTunnel(virConnectPtr conn,
                              virStreamPtr st,
                              unsigned long flags,
                              const char *dname,
                              unsigned long bandwidth,
                              const char *dom_xml)
{
    VIR_DEBUG("conn=%p, stream=%p, flags=%lx, dname=%s, "
              "bandwidth=%lu, dom_xml=%s", conn, st, flags,
              NULLSTR(dname), bandwidth, NULLSTR(dom_xml));

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn != st->conn) {
        virReportInvalidArg(conn, "%s",
                            _("conn must match stream connection"));
        goto error;
    }

    if (conn->driver->domainMigratePrepareTunnel) {
        int rv = conn->driver->domainMigratePrepareTunnel(conn, st,
                                                          flags, dname,
                                                          bandwidth, dom_xml);
        if (rv < 0)
            goto error;
        return rv;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
