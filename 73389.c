virConnectListDefinedDomains(virConnectPtr conn, char **const names,
                             int maxnames)
{
    VIR_DEBUG("conn=%p, names=%p, maxnames=%d", conn, names, maxnames);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckNonNullArgGoto(names, error);
    virCheckNonNegativeArgGoto(maxnames, error);

    if (conn->driver->connectListDefinedDomains) {
        int ret;
        ret = conn->driver->connectListDefinedDomains(conn, names, maxnames);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
