virDomainMigratePrepare2(virConnectPtr dconn,
                         char **cookie,
                         int *cookielen,
                         const char *uri_in,
                         char **uri_out,
                         unsigned long flags,
                         const char *dname,
                         unsigned long bandwidth,
                         const char *dom_xml)
{
    VIR_DEBUG("dconn=%p, cookie=%p, cookielen=%p, uri_in=%s, uri_out=%p,"
              "flags=%lx, dname=%s, bandwidth=%lu, dom_xml=%s", dconn,
              cookie, cookielen, NULLSTR(uri_in), uri_out, flags, NULLSTR(dname),
              bandwidth, NULLSTR(dom_xml));

    virResetLastError();

    virCheckConnectReturn(dconn, -1);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigratePrepare2) {
        int ret;
        ret = dconn->driver->domainMigratePrepare2(dconn, cookie, cookielen,
                                                   uri_in, uri_out,
                                                   flags, dname, bandwidth,
                                                   dom_xml);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return -1;
}
