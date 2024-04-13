virDomainFSThaw(virDomainPtr dom,
                const char **mountpoints,
                unsigned int nmountpoints,
                unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckReadOnlyGoto(dom->conn->flags, error);
    if (nmountpoints)
        virCheckNonNullArgGoto(mountpoints, error);
    else
        virCheckNullArgGoto(mountpoints, error);

    if (dom->conn->driver->domainFSThaw) {
        int ret = dom->conn->driver->domainFSThaw(
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
