virDomainScreenshot(virDomainPtr domain,
                    virStreamPtr stream,
                    unsigned int screen,
                    unsigned int flags)
{
    VIR_DOMAIN_DEBUG(domain, "stream=%p, flags=%x", stream, flags);

    virResetLastError();

    virCheckDomainReturn(domain, NULL);
    virCheckStreamGoto(stream, error);
    virCheckReadOnlyGoto(domain->conn->flags, error);

    if (domain->conn != stream->conn) {
        virReportInvalidArg(stream,
                            _("stream must match connection of domain '%s'"),
                            domain->name);
        goto error;
    }

    if (domain->conn->driver->domainScreenshot) {
        char *ret;
        ret = domain->conn->driver->domainScreenshot(domain, stream,
                                                     screen, flags);

        if (ret == NULL)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return NULL;
}
