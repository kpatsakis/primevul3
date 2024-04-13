virDomainMigrateVersion3Params(virDomainPtr domain,
                               virConnectPtr dconn,
                               virTypedParameterPtr params,
                               int nparams,
                               unsigned int flags)
{
    return virDomainMigrateVersion3Full(domain, dconn, NULL, NULL, NULL, 0,
                                        params, nparams, true, flags);
}
