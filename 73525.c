virDomainSendKey(virDomainPtr domain,
                 unsigned int codeset,
                 unsigned int holdtime,
                 unsigned int *keycodes,
                 int nkeycodes,
                 unsigned int flags)
{
    virConnectPtr conn;
    VIR_DOMAIN_DEBUG(domain, "codeset=%u, holdtime=%u, nkeycodes=%u, flags=%x",
                     codeset, holdtime, nkeycodes, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(keycodes, error);
    virCheckPositiveArgGoto(nkeycodes, error);

    if (nkeycodes > VIR_DOMAIN_SEND_KEY_MAX_KEYS) {
        virReportInvalidArg(nkeycodes,
                            _("nkeycodes must be <= %d"),
                            VIR_DOMAIN_SEND_KEY_MAX_KEYS);
        goto error;
    }

    if (conn->driver->domainSendKey) {
        int ret;
        ret = conn->driver->domainSendKey(domain, codeset, holdtime,
                                          keycodes, nkeycodes, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
