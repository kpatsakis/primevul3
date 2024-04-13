virDomainGetUUIDString(virDomainPtr domain, char *buf)
{
    VIR_DOMAIN_DEBUG(domain, "buf=%p", buf);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(buf, error);

    virUUIDFormat(domain->uuid, buf);
    return 0;

 error:
    virDispatchError(domain->conn);
    return -1;
}
