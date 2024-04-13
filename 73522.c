virDomainSaveImageDefineXML(virConnectPtr conn, const char *file,
                            const char *dxml, unsigned int flags)
{
    VIR_DEBUG("conn=%p, file=%s, dxml=%s, flags=%x",
              conn, NULLSTR(file), NULLSTR(dxml), flags);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(file, error);
    virCheckNonNullArgGoto(dxml, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_SAVE_RUNNING,
                             VIR_DOMAIN_SAVE_PAUSED,
                             error);

    if (conn->driver->domainSaveImageDefineXML) {
        int ret;
        char *absolute_file;

        /* We must absolutize the file path as the read is done out of process */
        if (virFileAbsPath(file, &absolute_file) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute input file path"));
            goto error;
        }

        ret = conn->driver->domainSaveImageDefineXML(conn, absolute_file,
                                                     dxml, flags);

        VIR_FREE(absolute_file);

        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
