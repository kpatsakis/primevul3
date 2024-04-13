virDomainOpenGraphicsFD(virDomainPtr dom,
                        unsigned int idx,
                        unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "idx=%u, flags=%x", idx, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);

    virCheckReadOnlyGoto(dom->conn->flags, error);

    if (!VIR_DRV_SUPPORTS_FEATURE(dom->conn->driver, dom->conn,
                                  VIR_DRV_FEATURE_FD_PASSING)) {
        virReportError(VIR_ERR_ARGUMENT_UNSUPPORTED, "%s",
                       _("fd passing is not supported by this connection"));
        goto error;
    }

    if (dom->conn->driver->domainOpenGraphicsFD) {
        int ret;
        ret = dom->conn->driver->domainOpenGraphicsFD(dom, idx, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
