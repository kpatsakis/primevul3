virDomainFSTrim(virDomainPtr dom,
                const char *mountPoint,
                unsigned long long minimum,
                unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "mountPoint=%s, minimum=%llu, flags=%x",
                     mountPoint, minimum, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckReadOnlyGoto(dom->conn->flags, error);

    if (dom->conn->driver->domainFSTrim) {
        int ret = dom->conn->driver->domainFSTrim(dom, mountPoint,
                                                  minimum, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
