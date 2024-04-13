virDomainListGetStats(virDomainPtr *doms,
                      unsigned int stats,
                      virDomainStatsRecordPtr **retStats,
                      unsigned int flags)
{
    virConnectPtr conn = NULL;
    virDomainPtr *nextdom = doms;
    unsigned int ndoms = 0;
    int ret = -1;

    VIR_DEBUG("doms=%p, stats=0x%x, retStats=%p, flags=0x%x",
              doms, stats, retStats, flags);

    virResetLastError();

    virCheckNonNullArgGoto(doms, cleanup);
    virCheckNonNullArgGoto(retStats, cleanup);

    if (!*doms) {
        virReportError(VIR_ERR_INVALID_ARG,
                       _("doms array in %s must contain at least one domain"),
                       __FUNCTION__);
        goto cleanup;
    }

    conn = doms[0]->conn;
    virCheckConnectReturn(conn, -1);

    if (!conn->driver->connectGetAllDomainStats) {
        virReportUnsupportedError();
        goto cleanup;
    }

    while (*nextdom) {
        virDomainPtr dom = *nextdom;

        virCheckDomainGoto(dom, cleanup);

        if (dom->conn != conn) {
            virReportError(VIR_ERR_INVALID_ARG, "%s",
                           _("domains in 'doms' array must belong to a "
                             "single connection"));
            goto cleanup;
        }

        ndoms++;
        nextdom++;
    }

    ret = conn->driver->connectGetAllDomainStats(conn, doms, ndoms,
                                                 stats, retStats, flags);

 cleanup:
    if (ret < 0)
        virDispatchError(conn);
    return ret;
}
