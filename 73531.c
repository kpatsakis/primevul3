virDomainSetMemoryFlags(virDomainPtr domain, unsigned long memory,
                        unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "memory=%lu, flags=%x", memory, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonZeroArgGoto(memory, error);

    if (virMemoryMaxValue(true) / 1024 <= memory) {
        virReportError(VIR_ERR_OVERFLOW, _("input too large: %lu"),
                       memory);
        goto error;
    }

    if (conn->driver->domainSetMemoryFlags) {
        int ret;
        ret = conn->driver->domainSetMemoryFlags(domain, memory, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
