virDomainIsUpdated(virDomainPtr dom)
{
    VIR_DOMAIN_DEBUG(dom);

    virResetLastError();

    virCheckDomainReturn(dom, -1);

    if (dom->conn->driver->domainIsUpdated) {
        int ret;
        ret = dom->conn->driver->domainIsUpdated(dom);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(dom->conn);
    return -1;
}
