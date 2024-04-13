virDomainOpenGraphics(virDomainPtr dom,
                      unsigned int idx,
                      int fd,
                      unsigned int flags)
{
    struct stat sb;
    VIR_DOMAIN_DEBUG(dom, "idx=%u, fd=%d, flags=%x",
                     idx, fd, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckNonNegativeArgGoto(fd, error);

    if (fstat(fd, &sb) < 0) {
        virReportSystemError(errno,
                             _("Unable to access file descriptor %d"), fd);
        goto error;
    }

    if (!S_ISSOCK(sb.st_mode)) {
        virReportInvalidArg(fd,
                            _("fd %d must be a socket"),
                            fd);
        goto error;
    }

    virCheckReadOnlyGoto(dom->conn->flags, error);

    if (!VIR_DRV_SUPPORTS_FEATURE(dom->conn->driver, dom->conn,
                                  VIR_DRV_FEATURE_FD_PASSING)) {
        virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                       _("fd passing is not supported by this connection"));
        goto error;
    }

    if (dom->conn->driver->domainOpenGraphics) {
        int ret;
        ret = dom->conn->driver->domainOpenGraphics(dom, idx, fd, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
