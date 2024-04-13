static uint8_t coupling_channel_element(NeAACDecStruct *hDecoder, bitfile *ld)
{
    uint8_t c, result = 0;
    uint8_t ind_sw_cce_flag = 0;
    uint8_t num_gain_element_lists = 0;
    uint8_t num_coupled_elements = 0;

    element el_empty = {0};
    ic_stream ics_empty = {0};
    int16_t sh_data[1024];

    c = faad_getbits(ld, LEN_TAG
        DEBUGVAR(1,900,"coupling_channel_element(): element_instance_tag"));

    ind_sw_cce_flag = faad_get1bit(ld
        DEBUGVAR(1,901,"coupling_channel_element(): ind_sw_cce_flag"));
    num_coupled_elements = faad_getbits(ld, 3
        DEBUGVAR(1,902,"coupling_channel_element(): num_coupled_elements"));

    for (c = 0; c < num_coupled_elements + 1; c++)
    {
        uint8_t cc_target_is_cpe, cc_target_tag_select;

        num_gain_element_lists++;

        cc_target_is_cpe = faad_get1bit(ld
            DEBUGVAR(1,903,"coupling_channel_element(): cc_target_is_cpe"));
        cc_target_tag_select = faad_getbits(ld, 4
            DEBUGVAR(1,904,"coupling_channel_element(): cc_target_tag_select"));

        if (cc_target_is_cpe)
        {
            uint8_t cc_l = faad_get1bit(ld
                DEBUGVAR(1,905,"coupling_channel_element(): cc_l"));
            uint8_t cc_r = faad_get1bit(ld
                DEBUGVAR(1,906,"coupling_channel_element(): cc_r"));

            if (cc_l && cc_r)
                num_gain_element_lists++;
        }
    }

    faad_get1bit(ld
        DEBUGVAR(1,907,"coupling_channel_element(): cc_domain"));
    faad_get1bit(ld
        DEBUGVAR(1,908,"coupling_channel_element(): gain_element_sign"));
    faad_getbits(ld, 2
        DEBUGVAR(1,909,"coupling_channel_element(): gain_element_scale"));

    if ((result = individual_channel_stream(hDecoder, &el_empty, ld, &ics_empty,
        0, sh_data)) > 0)
    {
        return result;
    }

    /* IS not allowed in single channel */
    if (ics->is_used)
        return 32;

    for (c = 1; c < num_gain_element_lists; c++)
    {
        uint8_t cge;

        if (ind_sw_cce_flag)
        {
            cge = 1;
        } else {
            cge = faad_get1bit(ld
                DEBUGVAR(1,910,"coupling_channel_element(): common_gain_element_present"));
        }

        if (cge)
        {
            huffman_scale_factor(ld);
        } else {
            uint8_t g, sfb;

            for (g = 0; g < ics_empty.num_window_groups; g++)
            {
                for (sfb = 0; sfb < ics_empty.max_sfb; sfb++)
                {
                    if (ics_empty.sfb_cb[g][sfb] != ZERO_HCB)
                        huffman_scale_factor(ld);
                }
            }
        }
    }

    return 0;
}
