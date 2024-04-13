static uint8_t ics_info(NeAACDecStruct *hDecoder, ic_stream *ics, bitfile *ld,
                        uint8_t common_window)
{
    uint8_t retval = 0;
    uint8_t ics_reserved_bit;

    ics_reserved_bit = faad_get1bit(ld
        DEBUGVAR(1,43,"ics_info(): ics_reserved_bit"));
    if (ics_reserved_bit != 0)
        return 32;
    ics->window_sequence = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,44,"ics_info(): window_sequence"));
    ics->window_shape = faad_get1bit(ld
        DEBUGVAR(1,45,"ics_info(): window_shape"));

#ifdef LD_DEC
    /* No block switching in LD */
    if ((hDecoder->object_type == LD) && (ics->window_sequence != ONLY_LONG_SEQUENCE))
        return 32;
#endif

    if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
    {
        ics->max_sfb = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,46,"ics_info(): max_sfb (short)"));
        ics->scale_factor_grouping = (uint8_t)faad_getbits(ld, 7
            DEBUGVAR(1,47,"ics_info(): scale_factor_grouping"));
    } else {
        ics->max_sfb = (uint8_t)faad_getbits(ld, 6
            DEBUGVAR(1,48,"ics_info(): max_sfb (long)"));
    }

    /* get the grouping information */
    if ((retval = window_grouping_info(hDecoder, ics)) > 0)
        return retval;


    /* should be an error */
    /* check the range of max_sfb */
    if (ics->max_sfb > ics->num_swb)
        return 16;

    if (ics->window_sequence != EIGHT_SHORT_SEQUENCE)
    {
        if ((ics->predictor_data_present = faad_get1bit(ld
            DEBUGVAR(1,49,"ics_info(): predictor_data_present"))) & 1)
        {
            if (hDecoder->object_type == MAIN) /* MPEG2 style AAC predictor */
            {
                uint8_t sfb;

                uint8_t limit = min(ics->max_sfb, max_pred_sfb(hDecoder->sf_index));
#ifdef MAIN_DEC
                ics->pred.limit = limit;
#endif

                if ((
#ifdef MAIN_DEC
                    ics->pred.predictor_reset =
#endif
                    faad_get1bit(ld DEBUGVAR(1,53,"ics_info(): pred.predictor_reset"))) & 1)
                {
#ifdef MAIN_DEC
                    ics->pred.predictor_reset_group_number =
#endif
                        (uint8_t)faad_getbits(ld, 5 DEBUGVAR(1,54,"ics_info(): pred.predictor_reset_group_number"));
                }

                for (sfb = 0; sfb < limit; sfb++)
                {
#ifdef MAIN_DEC
                    ics->pred.prediction_used[sfb] =
#endif
                        faad_get1bit(ld DEBUGVAR(1,55,"ics_info(): pred.prediction_used"));
                }
            }
#ifdef LTP_DEC
            else { /* Long Term Prediction */
                if (hDecoder->object_type < ER_OBJECT_START)
                {
                    if ((ics->ltp.data_present = faad_get1bit(ld
                        DEBUGVAR(1,50,"ics_info(): ltp.data_present"))) & 1)
                    {
                        if ((retval = ltp_data(hDecoder, ics, &(ics->ltp), ld)) > 0)
                        {
                            return retval;
                        }
                    }
                    if (common_window)
                    {
                        if ((ics->ltp2.data_present = faad_get1bit(ld
                            DEBUGVAR(1,51,"ics_info(): ltp2.data_present"))) & 1)
                        {
                            if ((retval = ltp_data(hDecoder, ics, &(ics->ltp2), ld)) > 0)
                            {
                                return retval;
                            }
                        }
                    }
                }
#ifdef ERROR_RESILIENCE
                if (!common_window && (hDecoder->object_type >= ER_OBJECT_START))
                {
                    if ((ics->ltp.data_present = faad_get1bit(ld
                        DEBUGVAR(1,50,"ics_info(): ltp.data_present"))) & 1)
                    {
                        ltp_data(hDecoder, ics, &(ics->ltp), ld);
                    }
                }
#endif
            }
#endif
        }
    }

    return retval;
}
