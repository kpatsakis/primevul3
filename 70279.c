static uint8_t pulse_data(ic_stream *ics, pulse_info *pul, bitfile *ld)
{
    uint8_t i;

    pul->number_pulse = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,56,"pulse_data(): number_pulse"));
    pul->pulse_start_sfb = (uint8_t)faad_getbits(ld, 6
        DEBUGVAR(1,57,"pulse_data(): pulse_start_sfb"));

    /* check the range of pulse_start_sfb */
    if (pul->pulse_start_sfb > ics->num_swb)
        return 16;

    for (i = 0; i < pul->number_pulse+1; i++)
    {
        pul->pulse_offset[i] = (uint8_t)faad_getbits(ld, 5
            DEBUGVAR(1,58,"pulse_data(): pulse_offset"));
#if 0
        printf("%d\n", pul->pulse_offset[i]);
#endif
        pul->pulse_amp[i] = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,59,"pulse_data(): pulse_amp"));
#if 0
        printf("%d\n", pul->pulse_amp[i]);
#endif
    }

    return 0;
}
