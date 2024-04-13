virDomainGetState(virDomainPtr domain,
                  int *state,
                  int *reason,
                  unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "state=%p, reason=%p, flags=%x",
                     state, reason, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    virCheckNonNullArgGoto(state, error);

    conn = domain->conn;
    if (conn->driver->domainGetState) {
        int ret;
        ret = conn->driver->domainGetState(domain, state, reason, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
