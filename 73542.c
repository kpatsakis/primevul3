virDomainStatsRecordListFree(virDomainStatsRecordPtr *stats)
{
    virDomainStatsRecordPtr *next;

    if (!stats)
        return;

    for (next = stats; *next; next++) {
        virTypedParamsFree((*next)->params, (*next)->nparams);
        virDomainFree((*next)->dom);
        VIR_FREE(*next);
    }

    VIR_FREE(stats);
}
