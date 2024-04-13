virDomainInterfaceStats(virDomainPtr dom, const char *path,
                        virDomainInterfaceStatsPtr stats, size_t size)
{
    virConnectPtr conn;
    virDomainInterfaceStatsStruct stats2 = { -1, -1, -1, -1,
                                             -1, -1, -1, -1 };

    VIR_DOMAIN_DEBUG(dom, "path=%s, stats=%p, size=%zi",
                     path, stats, size);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckNonNullArgGoto(path, error);
    virCheckNonNullArgGoto(stats, error);
    if (size > sizeof(stats2)) {
        virReportInvalidArg(size,
                            _("size must not exceed %zu"),
                            sizeof(stats2));
        goto error;
    }

    conn = dom->conn;

    if (conn->driver->domainInterfaceStats) {
        if (conn->driver->domainInterfaceStats(dom, path, &stats2) == -1)
            goto error;

        memcpy(stats, &stats2, size);
        return 0;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
