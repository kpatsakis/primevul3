static uint8_t program_config_element(program_config *pce, bitfile *ld)
{
    uint8_t i;

    memset(pce, 0, sizeof(program_config));

    pce->channels = 0;

    pce->element_instance_tag = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,10,"program_config_element(): element_instance_tag"));

    pce->object_type = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,11,"program_config_element(): object_type"));
    pce->sf_index = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,12,"program_config_element(): sf_index"));
    pce->num_front_channel_elements = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,13,"program_config_element(): num_front_channel_elements"));
    pce->num_side_channel_elements = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,14,"program_config_element(): num_side_channel_elements"));
    pce->num_back_channel_elements = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,15,"program_config_element(): num_back_channel_elements"));
    pce->num_lfe_channel_elements = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,16,"program_config_element(): num_lfe_channel_elements"));
    pce->num_assoc_data_elements = (uint8_t)faad_getbits(ld, 3
        DEBUGVAR(1,17,"program_config_element(): num_assoc_data_elements"));
    pce->num_valid_cc_elements = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,18,"program_config_element(): num_valid_cc_elements"));

    pce->mono_mixdown_present = faad_get1bit(ld
        DEBUGVAR(1,19,"program_config_element(): mono_mixdown_present"));
    if (pce->mono_mixdown_present == 1)
    {
        pce->mono_mixdown_element_number = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,20,"program_config_element(): mono_mixdown_element_number"));
    }

    pce->stereo_mixdown_present = faad_get1bit(ld
        DEBUGVAR(1,21,"program_config_element(): stereo_mixdown_present"));
    if (pce->stereo_mixdown_present == 1)
    {
        pce->stereo_mixdown_element_number = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,22,"program_config_element(): stereo_mixdown_element_number"));
    }

    pce->matrix_mixdown_idx_present = faad_get1bit(ld
        DEBUGVAR(1,23,"program_config_element(): matrix_mixdown_idx_present"));
    if (pce->matrix_mixdown_idx_present == 1)
    {
        pce->matrix_mixdown_idx = (uint8_t)faad_getbits(ld, 2
            DEBUGVAR(1,24,"program_config_element(): matrix_mixdown_idx"));
        pce->pseudo_surround_enable = faad_get1bit(ld
            DEBUGVAR(1,25,"program_config_element(): pseudo_surround_enable"));
    }

    for (i = 0; i < pce->num_front_channel_elements; i++)
    {
        pce->front_element_is_cpe[i] = faad_get1bit(ld
            DEBUGVAR(1,26,"program_config_element(): front_element_is_cpe"));
        pce->front_element_tag_select[i] = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,27,"program_config_element(): front_element_tag_select"));

        if (pce->front_element_is_cpe[i] & 1)
        {
            pce->cpe_channel[pce->front_element_tag_select[i]] = pce->channels;
            pce->num_front_channels += 2;
            pce->channels += 2;
        } else {
            pce->sce_channel[pce->front_element_tag_select[i]] = pce->channels;
            pce->num_front_channels++;
            pce->channels++;
        }
    }

    for (i = 0; i < pce->num_side_channel_elements; i++)
    {
        pce->side_element_is_cpe[i] = faad_get1bit(ld
            DEBUGVAR(1,28,"program_config_element(): side_element_is_cpe"));
        pce->side_element_tag_select[i] = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,29,"program_config_element(): side_element_tag_select"));

        if (pce->side_element_is_cpe[i] & 1)
        {
            pce->cpe_channel[pce->side_element_tag_select[i]] = pce->channels;
            pce->num_side_channels += 2;
            pce->channels += 2;
        } else {
            pce->sce_channel[pce->side_element_tag_select[i]] = pce->channels;
            pce->num_side_channels++;
            pce->channels++;
        }
    }

    for (i = 0; i < pce->num_back_channel_elements; i++)
    {
        pce->back_element_is_cpe[i] = faad_get1bit(ld
            DEBUGVAR(1,30,"program_config_element(): back_element_is_cpe"));
        pce->back_element_tag_select[i] = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,31,"program_config_element(): back_element_tag_select"));

        if (pce->back_element_is_cpe[i] & 1)
        {
            pce->cpe_channel[pce->back_element_tag_select[i]] = pce->channels;
            pce->channels += 2;
            pce->num_back_channels += 2;
        } else {
            pce->sce_channel[pce->back_element_tag_select[i]] = pce->channels;
            pce->num_back_channels++;
            pce->channels++;
        }
    }

    for (i = 0; i < pce->num_lfe_channel_elements; i++)
    {
        pce->lfe_element_tag_select[i] = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,32,"program_config_element(): lfe_element_tag_select"));

        pce->sce_channel[pce->lfe_element_tag_select[i]] = pce->channels;
        pce->num_lfe_channels++;
        pce->channels++;
    }

    for (i = 0; i < pce->num_assoc_data_elements; i++)
        pce->assoc_data_element_tag_select[i] = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,33,"program_config_element(): assoc_data_element_tag_select"));

    for (i = 0; i < pce->num_valid_cc_elements; i++)
    {
        pce->cc_element_is_ind_sw[i] = faad_get1bit(ld
            DEBUGVAR(1,34,"program_config_element(): cc_element_is_ind_sw"));
        pce->valid_cc_element_tag_select[i] = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,35,"program_config_element(): valid_cc_element_tag_select"));
    }

    faad_byte_align(ld);

    pce->comment_field_bytes = (uint8_t)faad_getbits(ld, 8
        DEBUGVAR(1,36,"program_config_element(): comment_field_bytes"));

    for (i = 0; i < pce->comment_field_bytes; i++)
    {
        pce->comment_field_data[i] = (uint8_t)faad_getbits(ld, 8
            DEBUGVAR(1,37,"program_config_element(): comment_field_data"));
    }
    pce->comment_field_data[i] = 0;

    if (pce->channels > MAX_CHANNELS)
        return 22;

    return 0;
}
