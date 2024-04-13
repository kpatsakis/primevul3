virDomainOpenConsole(virDomainPtr dom,
                     const char *dev_name,
                     virStreamPtr st,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "dev_name=%s, st=%p, flags=%x",
                     NULLSTR(dev_name), st, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckStreamGoto(st, error);
    virCheckReadOnlyGoto(conn->flags, error);

    if (conn != st->conn) {
        virReportInvalidArg(st,
                            _("stream must match connection of domain '%s'"),
                            dom->name);
        goto error;
    }

    if (conn->driver->domainOpenConsole) {
        int ret;
        ret = conn->driver->domainOpenConsole(dom, dev_name, st, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
