virDomainMigrateVersion2(virDomainPtr domain,
                         virConnectPtr dconn,
                         unsigned long flags,
                         const char *dname,
                         const char *uri,
                         unsigned long bandwidth)
{
    virDomainPtr ddomain = NULL;
    char *uri_out = NULL;
    char *cookie = NULL;
    char *dom_xml = NULL;
    int cookielen = 0, ret;
    virDomainInfo info;
    virErrorPtr orig_err = NULL;
    unsigned int getxml_flags = 0;
    int cancelled;
    unsigned long destflags;

    VIR_DOMAIN_DEBUG(domain,
                     "dconn=%p, flags=%lx, dname=%s, uri=%s, bandwidth=%lu",
                     dconn, flags, NULLSTR(dname), NULLSTR(uri), bandwidth);

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

    /* In version 2 of the protocol, the prepare step is slightly
     * different.  We fetch the domain XML of the source domain
     * and pass it to Prepare2.
     */
    if (!domain->conn->driver->domainGetXMLDesc) {
        virReportUnsupportedError();
        return NULL;
    }

    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_XML_MIGRATABLE)) {
        getxml_flags |= VIR_DOMAIN_XML_MIGRATABLE;
    } else {
        getxml_flags |= VIR_DOMAIN_XML_SECURE | VIR_DOMAIN_XML_UPDATE_CPU;
    }

    dom_xml = domain->conn->driver->domainGetXMLDesc(domain, getxml_flags);
    if (!dom_xml)
        return NULL;

    ret = virDomainGetInfo(domain, &info);
    if (ret == 0 && info.state == VIR_DOMAIN_PAUSED)
        flags |= VIR_MIGRATE_PAUSED;

    destflags = flags & ~(VIR_MIGRATE_ABORT_ON_ERROR |
                          VIR_MIGRATE_AUTO_CONVERGE);

    VIR_DEBUG("Prepare2 %p flags=%lx", dconn, destflags);
    ret = dconn->driver->domainMigratePrepare2
        (dconn, &cookie, &cookielen, uri, &uri_out, destflags, dname,
         bandwidth, dom_xml);
    VIR_FREE(dom_xml);
    if (ret == -1)
        goto done;

    if (uri == NULL && uri_out == NULL) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("domainMigratePrepare2 did not set uri"));
        cancelled = 1;
        /* Make sure Finish doesn't overwrite the error */
        orig_err = virSaveLastError();
        goto finish;
    }
    if (uri_out)
        uri = uri_out; /* Did domainMigratePrepare2 change URI? */

    /* Perform the migration.  The driver isn't supposed to return
     * until the migration is complete.
     */
    VIR_DEBUG("Perform %p", domain->conn);
    ret = domain->conn->driver->domainMigratePerform
        (domain, cookie, cookielen, uri, flags, dname, bandwidth);

    /* Perform failed. Make sure Finish doesn't overwrite the error */
    if (ret < 0)
        orig_err = virSaveLastError();

    /* If Perform returns < 0, then we need to cancel the VM
     * startup on the destination
     */
    cancelled = ret < 0 ? 1 : 0;

 finish:
    /* In version 2 of the migration protocol, we pass the
     * status code from the sender to the destination host,
     * so it can do any cleanup if the migration failed.
     */
    dname = dname ? dname : domain->name;
    VIR_DEBUG("Finish2 %p ret=%d", dconn, ret);
    ddomain = dconn->driver->domainMigrateFinish2
        (dconn, dname, cookie, cookielen, uri, destflags, cancelled);
    if (cancelled && ddomain)
        VIR_ERROR(_("finish step ignored that migration was cancelled"));

 done:
    if (orig_err) {
        virSetError(orig_err);
        virFreeError(orig_err);
    }
    VIR_FREE(uri_out);
    VIR_FREE(cookie);
    return ddomain;
}
