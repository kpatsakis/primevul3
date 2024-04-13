virDomainGetName(virDomainPtr domain)
{
    VIR_DEBUG("domain=%p", domain);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);

    return domain->name;
}
