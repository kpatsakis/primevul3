virDomainInterfaceAddresses(virDomainPtr dom,
                            virDomainInterfacePtr **ifaces,
                            unsigned int source,
                            unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "ifaces=%p, source=%d, flags=%x", ifaces, source, flags);

    virResetLastError();

    if (ifaces)
        *ifaces = NULL;
    virCheckDomainReturn(dom, -1);
    virCheckNonNullArgGoto(ifaces, error);
    if (source == VIR_DOMAIN_INTERFACE_ADDRESSES_SRC_AGENT)
        virCheckReadOnlyGoto(dom->conn->flags, error);

    if (dom->conn->driver->domainInterfaceAddresses) {
        int ret;
        ret = dom->conn->driver->domainInterfaceAddresses(dom, ifaces, source, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportError(VIR_ERR_NO_SUPPORT, __FUNCTION__);

 error:
    virDispatchError(dom->conn);
    return -1;
}
