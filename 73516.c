virDomainRename(virDomainPtr dom,
                const char *new_name,
                unsigned int flags)
{
    VIR_DEBUG("dom=%p, new_name=%s", dom, NULLSTR(new_name));

    virResetLastError();
    virCheckDomainReturn(dom, -1);
    virCheckNonNullArgGoto(new_name, error);
    virCheckReadOnlyGoto(dom->conn->flags, error);

    if (dom->conn->driver->domainRename) {
        int ret = dom->conn->driver->domainRename(dom, new_name, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(dom->conn);
    return -1;
}
