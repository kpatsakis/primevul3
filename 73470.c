virDomainManagedSave(virDomainPtr dom, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_SAVE_RUNNING,
                             VIR_DOMAIN_SAVE_PAUSED,
                             error);

    if (conn->driver->domainManagedSave) {
        int ret;

        ret = conn->driver->domainManagedSave(dom, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
