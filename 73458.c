virDomainIOThreadInfoFree(virDomainIOThreadInfoPtr info)
{
    if (!info)
        return;

    VIR_FREE(info->cpumap);
    VIR_FREE(info);
}
