virDomainLookupByUUIDString(virConnectPtr conn, const char *uuidstr)
{
    unsigned char uuid[VIR_UUID_BUFLEN];
    VIR_DEBUG("conn=%p, uuidstr=%s", conn, NULLSTR(uuidstr));

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNullArgGoto(uuidstr, error);

    if (virUUIDParse(uuidstr, uuid) < 0) {
        virReportInvalidArg(uuidstr, "%s", _("Invalid UUID"));
        goto error;
    }

    return virDomainLookupByUUID(conn, &uuid[0]);

 error:
    virDispatchError(conn);
    return NULL;
}
