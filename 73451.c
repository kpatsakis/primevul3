virDomainGetUUID(virDomainPtr domain, unsigned char *uuid)
{
    VIR_DOMAIN_DEBUG(domain, "uuid=%p", uuid);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(uuid, error);

    memcpy(uuid, &domain->uuid[0], VIR_UUID_BUFLEN);

    return 0;

 error:
    virDispatchError(domain->conn);
    return -1;
}
