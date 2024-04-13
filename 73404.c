virDomainBlockStats(virDomainPtr dom, const char *disk,
                    virDomainBlockStatsPtr stats, size_t size)
{
    virConnectPtr conn;
    virDomainBlockStatsStruct stats2 = { -1, -1, -1, -1, -1 };

    VIR_DOMAIN_DEBUG(dom, "disk=%s, stats=%p, size=%zi", disk, stats, size);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckNonNullArgGoto(disk, error);
    virCheckNonNullArgGoto(stats, error);
    if (size > sizeof(stats2)) {
        virReportInvalidArg(size,
                            _("size must not exceed %zu"),
                            sizeof(stats2));
        goto error;
    }
    conn = dom->conn;

    if (conn->driver->domainBlockStats) {
        if (conn->driver->domainBlockStats(dom, disk, &stats2) == -1)
            goto error;

        memcpy(stats, &stats2, size);
        return 0;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
