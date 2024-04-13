virDomainSaveImageGetXMLDesc(virConnectPtr conn, const char *file,
                             unsigned int flags)
{
    VIR_DEBUG("conn=%p, file=%s, flags=%x",
              conn, NULLSTR(file), flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNullArgGoto(file, error);

    if ((conn->flags & VIR_CONNECT_RO) && (flags & VIR_DOMAIN_XML_SECURE)) {
        virReportError(VIR_ERR_OPERATION_DENIED, "%s",
                       _("virDomainSaveImageGetXMLDesc with secure flag"));
        goto error;
    }

    if (conn->driver->domainSaveImageGetXMLDesc) {
        char *ret;
        char *absolute_file;

        /* We must absolutize the file path as the read is done out of process */
        if (virFileAbsPath(file, &absolute_file) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute input file path"));
            goto error;
        }

        ret = conn->driver->domainSaveImageGetXMLDesc(conn, absolute_file,
                                                      flags);

        VIR_FREE(absolute_file);

        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
