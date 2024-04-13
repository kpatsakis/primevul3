static uint8_t side_info(NeAACDecStruct *hDecoder, element *ele,
                         bitfile *ld, ic_stream *ics, uint8_t scal_flag)
{
    uint8_t result;

    ics->global_gain = (uint8_t)faad_getbits(ld, 8
        DEBUGVAR(1,67,"individual_channel_stream(): global_gain"));

    if (!ele->common_window && !scal_flag)
    {
        if ((result = ics_info(hDecoder, ics, ld, ele->common_window)) > 0)
            return result;
    }

    if ((result = section_data(hDecoder, ics, ld)) > 0)
        return result;

    if ((result = scale_factor_data(hDecoder, ics, ld)) > 0)
        return result;

    if (!scal_flag)
    {
        /**
         **  NOTE: It could be that pulse data is available in scalable AAC too,
         **        as said in Amendment 1, this could be only the case for ER AAC,
         **        though. (have to check this out later)
         **/
        /* get pulse data */
        if ((ics->pulse_data_present = faad_get1bit(ld
            DEBUGVAR(1,68,"individual_channel_stream(): pulse_data_present"))) & 1)
        {
            if ((result = pulse_data(ics, &(ics->pul), ld)) > 0)
                return result;
        }

        /* get tns data */
        if ((ics->tns_data_present = faad_get1bit(ld
            DEBUGVAR(1,69,"individual_channel_stream(): tns_data_present"))) & 1)
        {
#ifdef ERROR_RESILIENCE
            if (hDecoder->object_type < ER_OBJECT_START)
#endif
                tns_data(ics, &(ics->tns), ld);
        }

        /* get gain control data */
        if ((ics->gain_control_data_present = faad_get1bit(ld
            DEBUGVAR(1,70,"individual_channel_stream(): gain_control_data_present"))) & 1)
        {
#ifdef SSR_DEC
            if (hDecoder->object_type != SSR)
                return 1;
            else
                gain_control_data(ld, ics);
#else
            return 1;
#endif
        }
    }

#ifdef ERROR_RESILIENCE
    if (hDecoder->aacSpectralDataResilienceFlag)
    {
        ics->length_of_reordered_spectral_data = (uint16_t)faad_getbits(ld, 14
            DEBUGVAR(1,147,"individual_channel_stream(): length_of_reordered_spectral_data"));

        if (hDecoder->channelConfiguration == 2)
        {
            if (ics->length_of_reordered_spectral_data > 6144)
                ics->length_of_reordered_spectral_data = 6144;
        } else {
            if (ics->length_of_reordered_spectral_data > 12288)
                ics->length_of_reordered_spectral_data = 12288;
        }

        ics->length_of_longest_codeword = (uint8_t)faad_getbits(ld, 6
            DEBUGVAR(1,148,"individual_channel_stream(): length_of_longest_codeword"));
        if (ics->length_of_longest_codeword >= 49)
            ics->length_of_longest_codeword = 49;
    }

    /* RVLC spectral data is put here */
    if (hDecoder->aacScalefactorDataResilienceFlag)
    {
        if ((result = rvlc_decode_scale_factors(ics, ld)) > 0)
            return result;
    }
#endif

    return 0;
}
