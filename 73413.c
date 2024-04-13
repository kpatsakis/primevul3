virDomainCreateXMLWithFiles(virConnectPtr conn, const char *xmlDesc,
                            unsigned int nfiles,
                            int *files,
                            unsigned int flags)
{
    VIR_DEBUG("conn=%p, xmlDesc=%s, nfiles=%u, files=%p, flags=%x",
              conn, NULLSTR(xmlDesc), nfiles, files, flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNullArgGoto(xmlDesc, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainCreateXMLWithFiles) {
        virDomainPtr ret;
        ret = conn->driver->domainCreateXMLWithFiles(conn, xmlDesc,
                                                     nfiles, files,
                                                     flags);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return NULL;
}
