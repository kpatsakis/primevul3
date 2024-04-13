virDomainSetUserPassword(virDomainPtr dom,
                         const char *user,
                         const char *password,
                         unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "user=%s, password=%s, flags=%x",
                     NULLSTR(user), NULLSTR(password), flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    virCheckReadOnlyGoto(dom->conn->flags, error);
    virCheckNonNullArgGoto(user, error);
    virCheckNonNullArgGoto(password, error);

    if (dom->conn->driver->domainSetUserPassword) {
        int ret = dom->conn->driver->domainSetUserPassword(dom, user, password,
                                                           flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
