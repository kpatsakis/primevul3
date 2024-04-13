virDomainOpenChannel(virDomainPtr dom,
                     const char *name,
                     virStreamPtr st,
                     unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "name=%s, st=%p, flags=%x",
                     NULLSTR(name), st, flags);

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

    if (conn->driver->domainOpenChannel) {
        int ret;
        ret = conn->driver->domainOpenChannel(dom, name, st, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
