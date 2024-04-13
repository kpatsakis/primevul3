static uint8_t ltp_data(NeAACDecStruct *hDecoder, ic_stream *ics, ltp_info *ltp, bitfile *ld)
{
    uint8_t sfb, w;

    ltp->lag = 0;

#ifdef LD_DEC
    if (hDecoder->object_type == LD)
    {
        ltp->lag_update = (uint8_t)faad_getbits(ld, 1
            DEBUGVAR(1,142,"ltp_data(): lag_update"));

        if (ltp->lag_update)
        {
            ltp->lag = (uint16_t)faad_getbits(ld, 10
                DEBUGVAR(1,81,"ltp_data(): lag"));
        }
    } else {
#endif
        ltp->lag = (uint16_t)faad_getbits(ld, 11
            DEBUGVAR(1,81,"ltp_data(): lag"));
#ifdef LD_DEC
    }
#endif

    /* Check length of lag */
    if (ltp->lag > (hDecoder->frameLength << 1))
        return 18;

    ltp->coef = (uint8_t)faad_getbits(ld, 3
        DEBUGVAR(1,82,"ltp_data(): coef"));

    if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
    {
        for (w = 0; w < ics->num_windows; w++)
        {
            if ((ltp->short_used[w] = faad_get1bit(ld
                DEBUGVAR(1,83,"ltp_data(): short_used"))) & 1)
            {
                ltp->short_lag_present[w] = faad_get1bit(ld
                    DEBUGVAR(1,84,"ltp_data(): short_lag_present"));
                if (ltp->short_lag_present[w])
                {
                    ltp->short_lag[w] = (uint8_t)faad_getbits(ld, 4
                        DEBUGVAR(1,85,"ltp_data(): short_lag"));
                }
            }
        }
    } else {
        ltp->last_band = (ics->max_sfb < MAX_LTP_SFB ? ics->max_sfb : MAX_LTP_SFB);

        for (sfb = 0; sfb < ltp->last_band; sfb++)
        {
            ltp->long_used[sfb] = faad_get1bit(ld
                DEBUGVAR(1,86,"ltp_data(): long_used"));
        }
    }

    return 0;
}
