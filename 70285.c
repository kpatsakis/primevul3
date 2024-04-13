static uint8_t spectral_data(NeAACDecStruct *hDecoder, ic_stream *ics, bitfile *ld,
                             int16_t *spectral_data)
{
    int8_t i;
    uint8_t g;
    uint16_t inc, k, p = 0;
    uint8_t groups = 0;
    uint8_t sect_cb;
    uint8_t result;
    uint16_t nshort = hDecoder->frameLength/8;

#ifdef PROFILE
    int64_t count = faad_get_ts();
#endif

    for(g = 0; g < ics->num_window_groups; g++)
    {
        p = groups*nshort;

        for (i = 0; i < ics->num_sec[g]; i++)
        {
            sect_cb = ics->sect_cb[g][i];

            inc = (sect_cb >= FIRST_PAIR_HCB) ? 2 : 4;

            switch (sect_cb)
            {
            case ZERO_HCB:
            case NOISE_HCB:
            case INTENSITY_HCB:
            case INTENSITY_HCB2:
#ifdef SD_PRINT
                {
                    int j;
                    for (j = ics->sect_sfb_offset[g][ics->sect_start[g][i]]; j < ics->sect_sfb_offset[g][ics->sect_end[g][i]]; j++)
                    {
                        printf("%d\n", 0);
                    }
                }
#endif
#ifdef SFBO_PRINT
                printf("%d\n", ics->sect_sfb_offset[g][ics->sect_start[g][i]]);
#endif
                p += (ics->sect_sfb_offset[g][ics->sect_end[g][i]] -
                    ics->sect_sfb_offset[g][ics->sect_start[g][i]]);
                break;
            default:
#ifdef SFBO_PRINT
                printf("%d\n", ics->sect_sfb_offset[g][ics->sect_start[g][i]]);
#endif
                for (k = ics->sect_sfb_offset[g][ics->sect_start[g][i]];
                     k < ics->sect_sfb_offset[g][ics->sect_end[g][i]]; k += inc)
                {
                    if ((result = huffman_spectral_data(sect_cb, ld, &spectral_data[p])) > 0)
                        return result;
#ifdef SD_PRINT
                    {
                        int j;
                        for (j = p; j < p+inc; j++)
                        {
                            printf("%d\n", spectral_data[j]);
                        }
                    }
#endif
                    p += inc;
                }
                break;
            }
        }
        groups += ics->window_group_length[g];
    }

#ifdef PROFILE
    count = faad_get_ts() - count;
    hDecoder->spectral_cycles += count;
#endif

    return 0;
}
