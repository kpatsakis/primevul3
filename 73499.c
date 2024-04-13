virDomainMigrateUnmanagedParams(virDomainPtr domain,
                                const char *dconnuri,
                                virTypedParameterPtr params,
                                int nparams,
                                unsigned int flags)
{
    VIR_DOMAIN_DEBUG(domain, "dconnuri=%s, params=%p, nparams=%d, flags=%x",
                     NULLSTR(dconnuri), params, nparams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    if ((flags & VIR_MIGRATE_PEER2PEER) &&
        virDomainMigrateCheckNotLocal(dconnuri) < 0)
        return -1;

    if ((flags & VIR_MIGRATE_PEER2PEER) &&
        VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_MIGRATION_PARAMS)) {
        VIR_DEBUG("Using migration protocol 3 with extensible parameters");
        if (!domain->conn->driver->domainMigratePerform3Params) {
            virReportUnsupportedError();
            return -1;
        }
        return domain->conn->driver->domainMigratePerform3Params
                (domain, dconnuri, params, nparams,
                 NULL, 0, NULL, NULL, flags);
    } else if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                        VIR_DRV_FEATURE_MIGRATION_V3)) {
        VIR_DEBUG("Using migration protocol 3");
        if (!domain->conn->driver->domainMigratePerform3) {
            virReportUnsupportedError();
            return -1;
        }
        return virDomainMigrateUnmanagedProto3(domain, dconnuri,
                                               params, nparams, flags);
    } else {
        VIR_DEBUG("Using migration protocol 2");
        if (!domain->conn->driver->domainMigratePerform) {
            virReportUnsupportedError();
            return -1;
        }
        return virDomainMigrateUnmanagedProto2(domain, dconnuri,
                                               params, nparams, flags);
    }
}
