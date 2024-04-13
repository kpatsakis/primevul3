virDomainSave(virDomainPtr domain, const char *to)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "to=%s", to);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(to, error);

    if (conn->driver->domainSave) {
        int ret;
        char *absolute_to;

        /* We must absolutize the file path as the save is done out of process */
        if (virFileAbsPath(to, &absolute_to) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute output file path"));
            goto error;
        }

        ret = conn->driver->domainSave(domain, absolute_to);

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
