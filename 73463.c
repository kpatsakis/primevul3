virDomainIsActive(virDomainPtr dom)
{
    VIR_DEBUG("dom=%p", dom);

    virResetLastError();

    virCheckDomainReturn(dom, -1);

    if (dom->conn->driver->domainIsActive) {
        int ret;
        ret = dom->conn->driver->domainIsActive(dom);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(dom->conn);
    return -1;
}
