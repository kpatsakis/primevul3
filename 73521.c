virDomainSaveFlags(virDomainPtr domain, const char *to,
                   const char *dxml, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "to=%s, dxml=%s, flags=%x",
                     to, NULLSTR(dxml), flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(to, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_SAVE_RUNNING,
                             VIR_DOMAIN_SAVE_PAUSED,
                             error);

    if (conn->driver->domainSaveFlags) {
        int ret;
        char *absolute_to;

        /* We must absolutize the file path as the save is done out of process */
        if (virFileAbsPath(to, &absolute_to) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute output file path"));
            goto error;
        }

        ret = conn->driver->domainSaveFlags(domain, absolute_to, dxml, flags);

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
