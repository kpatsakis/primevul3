virDomainSetMaxMemory(virDomainPtr domain, unsigned long memory)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "memory=%lu", memory);

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

    if (conn->driver->domainSetMaxMemory) {
        int ret;
        ret = conn->driver->domainSetMaxMemory(domain, memory);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
