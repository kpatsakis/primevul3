virDomainRestoreFlags(virConnectPtr conn, const char *from, const char *dxml,
                      unsigned int flags)
{
    VIR_DEBUG("conn=%p, from=%s, dxml=%s, flags=%x",
              conn, NULLSTR(from), NULLSTR(dxml), flags);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(from, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_SAVE_RUNNING,
                             VIR_DOMAIN_SAVE_PAUSED,
                             error);

    if (conn->driver->domainRestoreFlags) {
        int ret;
        char *absolute_from;

        /* We must absolutize the file path as the restore is done out of process */
        if (virFileAbsPath(from, &absolute_from) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute input file path"));
            goto error;
        }

        ret = conn->driver->domainRestoreFlags(conn, absolute_from, dxml,
                                               flags);

        VIR_FREE(absolute_from);

        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
