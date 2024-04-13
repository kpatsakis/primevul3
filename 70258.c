static void adts_error_check(adts_header *adts, bitfile *ld)
{
    if (adts->protection_absent == 0)
    {
        adts->crc_check = (uint16_t)faad_getbits(ld, 16
            DEBUGVAR(1,134,"adts_error_check(): crc_check"));
    }
}
