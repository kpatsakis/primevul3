static uint8_t fill_element(NeAACDecStruct *hDecoder, bitfile *ld, drc_info *drc
#ifdef SBR_DEC
                            ,uint8_t sbr_ele
#endif
                            )
{
    uint16_t count;
#ifdef SBR_DEC
    uint8_t bs_extension_type;
#endif

    count = (uint16_t)faad_getbits(ld, 4
        DEBUGVAR(1,65,"fill_element(): count"));
    if (count == 15)
    {
        count += (uint16_t)faad_getbits(ld, 8
            DEBUGVAR(1,66,"fill_element(): extra count")) - 1;
    }

    if (count > 0)
    {
#ifdef SBR_DEC
        bs_extension_type = (uint8_t)faad_showbits(ld, 4);

        if ((bs_extension_type == EXT_SBR_DATA) ||
            (bs_extension_type == EXT_SBR_DATA_CRC))
        {
            if (sbr_ele == INVALID_SBR_ELEMENT)
                return 24;

            if (!hDecoder->sbr[sbr_ele])
            {
                hDecoder->sbr[sbr_ele] = sbrDecodeInit(hDecoder->frameLength,
                    hDecoder->element_id[sbr_ele], 2*get_sample_rate(hDecoder->sf_index),
                    hDecoder->downSampledSBR
#ifdef DRM
                    , 0
#endif
                    );
            }

            hDecoder->sbr_present_flag = 1;

            /* parse the SBR data */
            hDecoder->sbr[sbr_ele]->ret = sbr_extension_data(ld, hDecoder->sbr[sbr_ele], count,
                hDecoder->postSeekResetFlag);

#if 0
            if (hDecoder->sbr[sbr_ele]->ret > 0)
            {
                printf("%s\n", NeAACDecGetErrorMessage(hDecoder->sbr[sbr_ele]->ret));
            }
#endif

#if (defined(PS_DEC) || defined(DRM_PS))
            if (hDecoder->sbr[sbr_ele]->ps_used)
            {
                hDecoder->ps_used[sbr_ele] = 1;

                /* set element independent flag to 1 as well */
                hDecoder->ps_used_global = 1;
            }
#endif
        } else {
#endif
#ifndef DRM
            while (count > 0)
            {
                count -= extension_payload(ld, drc, count);
            }
#else
            return 30;
#endif
#ifdef SBR_DEC
        }
#endif
    }

    return 0;
}
