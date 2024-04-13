virDomainGetID(virDomainPtr domain)
{
    VIR_DOMAIN_DEBUG(domain);

    virResetLastError();

    virCheckDomainReturn(domain, (unsigned int)-1);

    return domain->id;
}
