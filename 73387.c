virConnectGetDomainCapabilities(virConnectPtr conn,
                                const char *emulatorbin,
                                const char *arch,
                                const char *machine,
                                const char *virttype,
                                unsigned int flags)
{
    VIR_DEBUG("conn=%p, emulatorbin=%s, arch=%s, "
              "machine=%s, virttype=%s, flags=%x",
              conn, NULLSTR(emulatorbin), NULLSTR(arch),
              NULLSTR(machine), NULLSTR(virttype), flags);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);

    if (conn->driver->connectGetDomainCapabilities) {
        char *ret;
        ret = conn->driver->connectGetDomainCapabilities(conn, emulatorbin,
                                                         arch, machine,
                                                         virttype, flags);
        if (!ret)
            goto error;
        VIR_DEBUG("conn=%p, ret=%s", conn, ret);
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
