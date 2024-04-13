virDomainMigrateFinish2(virConnectPtr dconn,
                        const char *dname,
                        const char *cookie,
                        int cookielen,
                        const char *uri,
                        unsigned long flags,
                        int retcode)
{
    VIR_DEBUG("dconn=%p, dname=%s, cookie=%p, cookielen=%d, uri=%s, "
              "flags=%lx, retcode=%d", dconn, NULLSTR(dname), cookie,
              cookielen, NULLSTR(uri), flags, retcode);

    virResetLastError();

    virCheckConnectReturn(dconn, NULL);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigrateFinish2) {
        virDomainPtr ret;
        ret = dconn->driver->domainMigrateFinish2(dconn, dname,
                                                  cookie, cookielen,
                                                  uri, flags,
                                                  retcode);
        if (!ret && !retcode)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return NULL;
}
