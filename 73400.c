virDomainBlockPeek(virDomainPtr dom,
                   const char *disk,
                   unsigned long long offset /* really 64 bits */,
                   size_t size,
                   void *buffer,
                   unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "disk=%s, offset=%lld, size=%zi, buffer=%p, flags=%x",
                     disk, offset, size, buffer, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonEmptyStringArgGoto(disk, error);

    /* Allow size == 0 as an access test. */
    if (size > 0)
        virCheckNonNullArgGoto(buffer, error);

    if (conn->driver->domainBlockPeek) {
        int ret;
        ret = conn->driver->domainBlockPeek(dom, disk, offset, size,
                                            buffer, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
