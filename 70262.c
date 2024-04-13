static uint8_t channel_pair_element(NeAACDecStruct *hDecoder, bitfile *ld,
                                    uint8_t channels, uint8_t *tag)
{
    ALIGN int16_t spec_data1[1024] = {0};
    ALIGN int16_t spec_data2[1024] = {0};
    element cpe = {0};
    ic_stream *ics1 = &(cpe.ics1);
    ic_stream *ics2 = &(cpe.ics2);
    uint8_t result;

    cpe.channel        = channels;
    cpe.paired_channel = channels+1;

    cpe.element_instance_tag = (uint8_t)faad_getbits(ld, LEN_TAG
        DEBUGVAR(1,39,"channel_pair_element(): element_instance_tag"));
    *tag = cpe.element_instance_tag;

    if ((cpe.common_window = faad_get1bit(ld
        DEBUGVAR(1,40,"channel_pair_element(): common_window"))) & 1)
    {
        /* both channels have common ics information */
        if ((result = ics_info(hDecoder, ics1, ld, cpe.common_window)) > 0)
            return result;

        ics1->ms_mask_present = (uint8_t)faad_getbits(ld, 2
            DEBUGVAR(1,41,"channel_pair_element(): ms_mask_present"));
        if (ics1->ms_mask_present == 3)
        {
            /* bitstream error */
            return 32;
        }
        if (ics1->ms_mask_present == 1)
        {
            uint8_t g, sfb;
            for (g = 0; g < ics1->num_window_groups; g++)
            {
                for (sfb = 0; sfb < ics1->max_sfb; sfb++)
                {
                    ics1->ms_used[g][sfb] = faad_get1bit(ld
                        DEBUGVAR(1,42,"channel_pair_element(): faad_get1bit"));
                }
            }
        }

#ifdef ERROR_RESILIENCE
        if ((hDecoder->object_type >= ER_OBJECT_START) && (ics1->predictor_data_present))
        {
            if ((
#ifdef LTP_DEC
                ics1->ltp.data_present =
#endif
                faad_get1bit(ld DEBUGVAR(1,50,"channel_pair_element(): ltp.data_present"))) & 1)
            {
#ifdef LTP_DEC
                if ((result = ltp_data(hDecoder, ics1, &(ics1->ltp), ld)) > 0)
                {
                    return result;
                }
#else
                return 26;
#endif
            }
        }
#endif

        memcpy(ics2, ics1, sizeof(ic_stream));
    } else {
        ics1->ms_mask_present = 0;
    }

    if ((result = individual_channel_stream(hDecoder, &cpe, ld, ics1,
        0, spec_data1)) > 0)
    {
        return result;
    }

#ifdef ERROR_RESILIENCE
    if (cpe.common_window && (hDecoder->object_type >= ER_OBJECT_START) &&
        (ics1->predictor_data_present))
    {
        if ((
#ifdef LTP_DEC
            ics1->ltp2.data_present =
#endif
            faad_get1bit(ld DEBUGVAR(1,50,"channel_pair_element(): ltp.data_present"))) & 1)
        {
#ifdef LTP_DEC
            if ((result = ltp_data(hDecoder, ics1, &(ics1->ltp2), ld)) > 0)
            {
                return result;
            }
#else
            return 26;
#endif
        }
    }
#endif

    if ((result = individual_channel_stream(hDecoder, &cpe, ld, ics2,
        0, spec_data2)) > 0)
    {
        return result;
    }

#ifdef SBR_DEC
    /* check if next bitstream element is a fill element */
    /* if so, read it now so SBR decoding can be done in case of a file with SBR */
    if (faad_showbits(ld, LEN_SE_ID) == ID_FIL)
    {
        faad_flushbits(ld, LEN_SE_ID);

        /* one sbr_info describes a channel_element not a channel! */
        if ((result = fill_element(hDecoder, ld, hDecoder->drc, hDecoder->fr_ch_ele)) > 0)
        {
            return result;
        }
    }
#endif

    /* noiseless coding is done, spectral reconstruction is done now */
    if ((result = reconstruct_channel_pair(hDecoder, ics1, ics2, &cpe,
        spec_data1, spec_data2)) > 0)
    {
        return result;
    }

    return 0;
}
