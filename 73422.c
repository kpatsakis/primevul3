virDomainFSInfoFree(virDomainFSInfoPtr info)
{
    size_t i;

    if (!info)
        return;

    VIR_FREE(info->mountpoint);
    VIR_FREE(info->name);
    VIR_FREE(info->fstype);

    for (i = 0; i < info->ndevAlias; i++)
        VIR_FREE(info->devAlias[i]);
    VIR_FREE(info->devAlias);

    VIR_FREE(info);
}
