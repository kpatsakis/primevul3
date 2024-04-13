virDomainFSFreeze(virDomainPtr dom,
                  const char **mountpoints,
                  unsigned int nmountpoints,
                  unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "mountpoints=%p, nmountpoints=%d, flags=%x",
                     mountpoints, nmountpoints, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckReadOnlyGoto(dom->conn->flags, error);
    if (nmountpoints)
        virCheckNonNullArgGoto(mountpoints, error);
    else
        virCheckNullArgGoto(mountpoints, error);

    if (dom->conn->driver->domainFSFreeze) {
        int ret = dom->conn->driver->domainFSFreeze(
            dom, mountpoints, nmountpoints, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
