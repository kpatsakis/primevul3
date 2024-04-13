virDomainMigrateVersion1(virDomainPtr domain,
                         virConnectPtr dconn,
                         unsigned long flags,
                         const char *dname,
                         const char *uri,
                         unsigned long bandwidth)
{
    virDomainPtr ddomain = NULL;
    char *uri_out = NULL;
    char *cookie = NULL;
    int cookielen = 0, ret;
    virDomainInfo info;
    unsigned int destflags;

    VIR_DOMAIN_DEBUG(domain,
                     "dconn=%p, flags=%lx, dname=%s, uri=%s, bandwidth=%lu",
                     dconn, flags, NULLSTR(dname), NULLSTR(uri), bandwidth);

    ret = virDomainGetInfo(domain, &info);
    if (ret == 0 && info.state == VIR_DOMAIN_PAUSED)
        flags |= VIR_MIGRATE_PAUSED;

    destflags = flags & ~(VIR_MIGRATE_ABORT_ON_ERROR |
                          VIR_MIGRATE_AUTO_CONVERGE);

    /* Prepare the migration.
     *
     * The destination host may return a cookie, or leave cookie as
     * NULL.
     *
     * The destination host MUST set uri_out if uri_in is NULL.
     *
     * If uri_in is non-NULL, then the destination host may modify
     * the URI by setting uri_out.  If it does not wish to modify
     * the URI, it should leave uri_out as NULL.
     */
    if (dconn->driver->domainMigratePrepare
        (dconn, &cookie, &cookielen, uri, &uri_out, destflags, dname,
         bandwidth) == -1)
        goto done;

    if (uri == NULL && uri_out == NULL) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("domainMigratePrepare did not set uri"));
        goto done;
    }
    if (uri_out)
        uri = uri_out; /* Did domainMigratePrepare change URI? */

    /* Perform the migration.  The driver isn't supposed to return
     * until the migration is complete.
     */
    if (domain->conn->driver->domainMigratePerform
        (domain, cookie, cookielen, uri, flags, dname, bandwidth) == -1)
        goto done;

    /* Get the destination domain and return it or error.
     * 'domain' no longer actually exists at this point
     * (or so we hope), but we still use the object in memory
     * in order to get the name.
     */
    dname = dname ? dname : domain->name;
    if (dconn->driver->domainMigrateFinish)
        ddomain = dconn->driver->domainMigrateFinish
            (dconn, dname, cookie, cookielen, uri, destflags);
    else
        ddomain = virDomainLookupByName(dconn, dname);

 done:
    VIR_FREE(uri_out);
    VIR_FREE(cookie);
    return ddomain;
}
