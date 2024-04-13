virDomainCreateWithFiles(virDomainPtr domain, unsigned int nfiles,
                         int *files, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "nfiles=%u, files=%p, flags=%x",
                     nfiles, files, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainCreateWithFiles) {
        int ret;
        ret = conn->driver->domainCreateWithFiles(domain,
                                                  nfiles, files,
                                                  flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
