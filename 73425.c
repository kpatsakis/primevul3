virDomainFree(virDomainPtr domain)
{
    VIR_DOMAIN_DEBUG(domain);

    virResetLastError();

    virCheckDomainReturn(domain, -1);

    virObjectUnref(domain);
    return 0;
}
