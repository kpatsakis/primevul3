virDomainHasManagedSaveImage(virDomainPtr dom, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "flags=%x", flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    if (conn->driver->domainHasManagedSaveImage) {
        int ret;

        ret = conn->driver->domainHasManagedSaveImage(dom, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
