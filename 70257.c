static int8_t DRM_aac_scalable_main_header(NeAACDecStruct *hDecoder, ic_stream *ics1, ic_stream *ics2,
                                           bitfile *ld, uint8_t this_layer_stereo)
{
    uint8_t retval = 0;
    uint8_t ch;
    ic_stream *ics;
    uint8_t ics_reserved_bit;

    ics_reserved_bit = faad_get1bit(ld
        DEBUGVAR(1,300,"aac_scalable_main_header(): ics_reserved_bits"));
    if (ics_reserved_bit != 0)
        return 32;
    ics1->window_sequence = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,301,"aac_scalable_main_header(): window_sequence"));
    ics1->window_shape = faad_get1bit(ld
        DEBUGVAR(1,302,"aac_scalable_main_header(): window_shape"));

    if (ics1->window_sequence == EIGHT_SHORT_SEQUENCE)
    {
        ics1->max_sfb = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,303,"aac_scalable_main_header(): max_sfb (short)"));
        ics1->scale_factor_grouping = (uint8_t)faad_getbits(ld, 7
            DEBUGVAR(1,304,"aac_scalable_main_header(): scale_factor_grouping"));
    } else {
        ics1->max_sfb = (uint8_t)faad_getbits(ld, 6
            DEBUGVAR(1,305,"aac_scalable_main_header(): max_sfb (long)"));
    }

    /* get the grouping information */
    if ((retval = window_grouping_info(hDecoder, ics1)) > 0)
        return retval;

    /* should be an error */
    /* check the range of max_sfb */
    if (ics1->max_sfb > ics1->num_swb)
        return 16;

    if (this_layer_stereo)
    {
        ics1->ms_mask_present = (uint8_t)faad_getbits(ld, 2
            DEBUGVAR(1,306,"aac_scalable_main_header(): ms_mask_present"));
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
                        DEBUGVAR(1,307,"aac_scalable_main_header(): faad_get1bit"));
                }
            }
        }

        memcpy(ics2, ics1, sizeof(ic_stream));
    } else {
        ics1->ms_mask_present = 0;
    }

    return 0;
}
