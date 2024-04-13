virDomainMemoryStats(virDomainPtr dom, virDomainMemoryStatPtr stats,
                     unsigned int nr_stats, unsigned int flags)
{
    virConnectPtr conn;
    unsigned long nr_stats_ret = 0;

    VIR_DOMAIN_DEBUG(dom, "stats=%p, nr_stats=%u, flags=%x",
                     stats, nr_stats, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);

    if (!stats || nr_stats == 0)
        return 0;

    if (nr_stats > VIR_DOMAIN_MEMORY_STAT_NR)
        nr_stats = VIR_DOMAIN_MEMORY_STAT_NR;

    conn = dom->conn;
    if (conn->driver->domainMemoryStats) {
        nr_stats_ret = conn->driver->domainMemoryStats(dom, stats, nr_stats,
                                                       flags);
        if (nr_stats_ret == -1)
            goto error;
        return nr_stats_ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
