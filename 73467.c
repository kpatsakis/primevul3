virDomainLookupByName(virConnectPtr conn, const char *name)
{
    VIR_DEBUG("conn=%p, name=%s", conn, NULLSTR(name));

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNullArgGoto(name, error);

    if (conn->driver->domainLookupByName) {
        virDomainPtr dom;
        dom = conn->driver->domainLookupByName(conn, name);
        if (!dom)
            goto error;
        return dom;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
