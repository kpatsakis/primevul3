virDomainCoreDump(virDomainPtr domain, const char *to, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "to=%s, flags=%x", to, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(to, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DUMP_CRASH, VIR_DUMP_LIVE, error);
    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DUMP_CRASH, VIR_DUMP_RESET, error);
    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DUMP_LIVE, VIR_DUMP_RESET, error);

    if (conn->driver->domainCoreDump) {
        int ret;
        char *absolute_to;

        /* We must absolutize the file path as the save is done out of process */
        if (virFileAbsPath(to, &absolute_to) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute core file path"));
            goto error;
        }

        ret = conn->driver->domainCoreDump(domain, absolute_to, flags);

        VIR_FREE(absolute_to);

        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
