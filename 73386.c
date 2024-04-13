virConnectGetAllDomainStats(virConnectPtr conn,
                            unsigned int stats,
                            virDomainStatsRecordPtr **retStats,
                            unsigned int flags)
{
    int ret = -1;

    VIR_DEBUG("conn=%p, stats=0x%x, retStats=%p, flags=0x%x",
              conn, stats, retStats, flags);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckNonNullArgGoto(retStats, cleanup);

    if (!conn->driver->connectGetAllDomainStats) {
        virReportUnsupportedError();
        goto cleanup;
    }

    ret = conn->driver->connectGetAllDomainStats(conn, NULL, 0, stats,
                                                 retStats, flags);

 cleanup:
    if (ret < 0)
        virDispatchError(conn);

    return ret;
}
