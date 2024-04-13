virDomainLookupByUUID(virConnectPtr conn, const unsigned char *uuid)
{
    VIR_UUID_DEBUG(conn, uuid);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNullArgGoto(uuid, error);

    if (conn->driver->domainLookupByUUID) {
        virDomainPtr ret;
        ret = conn->driver->domainLookupByUUID(conn, uuid);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
