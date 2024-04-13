static uint8_t individual_channel_stream(NeAACDecStruct *hDecoder, element *ele,
                                         bitfile *ld, ic_stream *ics, uint8_t scal_flag,
                                         int16_t *spec_data)
{
    uint8_t result;

    result = side_info(hDecoder, ele, ld, ics, scal_flag);
    if (result > 0)
        return result;

    if (hDecoder->object_type >= ER_OBJECT_START) 
    {
        if (ics->tns_data_present)
            tns_data(ics, &(ics->tns), ld);
    }

#ifdef DRM
    /* CRC check */
    if (hDecoder->object_type == DRM_ER_LC)
    {
        if ((result = (uint8_t)faad_check_CRC(ld, (uint16_t)faad_get_processed_bits(ld) - 8)) > 0)
            return result;
    }
#endif

#ifdef ERROR_RESILIENCE
    if (hDecoder->aacSpectralDataResilienceFlag)
    {
        /* error resilient spectral data decoding */
        if ((result = reordered_spectral_data(hDecoder, ics, ld, spec_data)) > 0)
        {
            return result;
        }
    } else {
#endif
        /* decode the spectral data */
        if ((result = spectral_data(hDecoder, ics, ld, spec_data)) > 0)
        {
            return result;
        }
#ifdef ERROR_RESILIENCE
    }
#endif

    /* pulse coding reconstruction */
    if (ics->pulse_data_present)
    {
        if (ics->window_sequence != EIGHT_SHORT_SEQUENCE)
        {
            if ((result = pulse_decode(ics, spec_data, hDecoder->frameLength)) > 0)
                return result;
        } else {
            return 2; /* pulse coding not allowed for short blocks */
        }
    }

    return 0;
}
