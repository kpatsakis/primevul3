virDomainMigrateFinish(virConnectPtr dconn,
                       const char *dname,
                       const char *cookie,
                       int cookielen,
                       const char *uri,
                       unsigned long flags)
{
    VIR_DEBUG("dconn=%p, dname=%s, cookie=%p, cookielen=%d, uri=%s, "
              "flags=%lx", dconn, NULLSTR(dname), cookie, cookielen,
              NULLSTR(uri), flags);

    virResetLastError();

    virCheckConnectReturn(dconn, NULL);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigrateFinish) {
        virDomainPtr ret;
        ret = dconn->driver->domainMigrateFinish(dconn, dname,
                                                 cookie, cookielen,
                                                 uri, flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return NULL;
}
