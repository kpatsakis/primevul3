virDomainRef(virDomainPtr domain)
{
    VIR_DOMAIN_DEBUG(domain, "refs=%d", domain ? domain->object.u.s.refs : 0);

    virResetLastError();

    virCheckDomainReturn(domain, -1);

    virObjectRef(domain);
    return 0;
}
