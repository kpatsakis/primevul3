virDomainGetSchedulerType(virDomainPtr domain, int *nparams)
{
    virConnectPtr conn;
    char *schedtype;

    VIR_DOMAIN_DEBUG(domain, "nparams=%p", nparams);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    conn = domain->conn;

    if (conn->driver->domainGetSchedulerType) {
        schedtype = conn->driver->domainGetSchedulerType(domain, nparams);
        if (!schedtype)
            goto error;
        return schedtype;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
