virDomainMigrateFinish3(virConnectPtr dconn,
                        const char *dname,
                        const char *cookiein,
                        int cookieinlen,
                        char **cookieout,
                        int *cookieoutlen,
                        const char *dconnuri,
                        const char *uri,
                        unsigned long flags,
                        int cancelled)
{
    VIR_DEBUG("dconn=%p, dname=%s, cookiein=%p, cookieinlen=%d, cookieout=%p,"
              "cookieoutlen=%p, dconnuri=%s, uri=%s, flags=%lx, retcode=%d",
              dconn, NULLSTR(dname), cookiein, cookieinlen, cookieout,
              cookieoutlen, NULLSTR(dconnuri), NULLSTR(uri), flags, cancelled);

    virResetLastError();

    virCheckConnectReturn(dconn, NULL);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigrateFinish3) {
        virDomainPtr ret;
        ret = dconn->driver->domainMigrateFinish3(dconn, dname,
                                                  cookiein, cookieinlen,
                                                  cookieout, cookieoutlen,
                                                  dconnuri, uri, flags,
                                                  cancelled);
        if (!ret && !cancelled)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return NULL;
}
