static uint8_t adts_fixed_header(adts_header *adts, bitfile *ld)
{
    uint16_t i;
    uint8_t sync_err = 1;

    /* try to recover from sync errors */
    for (i = 0; i < 768; i++)
    {
        adts->syncword = (uint16_t)faad_showbits(ld, 12);
        if (adts->syncword != 0xFFF)
        {
            faad_getbits(ld, 8
                DEBUGVAR(0,0,""));
        } else {
            sync_err = 0;
            faad_getbits(ld, 12
                DEBUGVAR(1,118,"adts_fixed_header(): syncword"));
            break;
        }
    }
    if (sync_err)
        return 5;

    adts->id = faad_get1bit(ld
        DEBUGVAR(1,119,"adts_fixed_header(): id"));
    adts->layer = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,120,"adts_fixed_header(): layer"));
    adts->protection_absent = faad_get1bit(ld
        DEBUGVAR(1,121,"adts_fixed_header(): protection_absent"));
    adts->profile = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,122,"adts_fixed_header(): profile"));
    adts->sf_index = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,123,"adts_fixed_header(): sf_index"));
    adts->private_bit = faad_get1bit(ld
        DEBUGVAR(1,124,"adts_fixed_header(): private_bit"));
    adts->channel_configuration = (uint8_t)faad_getbits(ld, 3
        DEBUGVAR(1,125,"adts_fixed_header(): channel_configuration"));
    adts->original = faad_get1bit(ld
        DEBUGVAR(1,126,"adts_fixed_header(): original"));
    adts->home = faad_get1bit(ld
        DEBUGVAR(1,127,"adts_fixed_header(): home"));

    if (adts->old_format == 1)
    {
        /* Removed in corrigendum 14496-3:2002 */
        if (adts->id == 0)
        {
            adts->emphasis = (uint8_t)faad_getbits(ld, 2
                DEBUGVAR(1,128,"adts_fixed_header(): emphasis"));
        }
    }

    return 0;
}
