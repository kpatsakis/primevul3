void get_adif_header(adif_header *adif, bitfile *ld)
{
    uint8_t i;

    /* adif_id[0] = */ faad_getbits(ld, 8
        DEBUGVAR(1,106,"get_adif_header(): adif_id[0]"));
    /* adif_id[1] = */ faad_getbits(ld, 8
        DEBUGVAR(1,107,"get_adif_header(): adif_id[1]"));
    /* adif_id[2] = */ faad_getbits(ld, 8
        DEBUGVAR(1,108,"get_adif_header(): adif_id[2]"));
    /* adif_id[3] = */ faad_getbits(ld, 8
        DEBUGVAR(1,109,"get_adif_header(): adif_id[3]"));
    adif->copyright_id_present = faad_get1bit(ld
        DEBUGVAR(1,110,"get_adif_header(): copyright_id_present"));
    if(adif->copyright_id_present)
    {
        for (i = 0; i < 72/8; i++)
        {
            adif->copyright_id[i] = (int8_t)faad_getbits(ld, 8
                DEBUGVAR(1,111,"get_adif_header(): copyright_id"));
        }
        adif->copyright_id[i] = 0;
    }
    adif->original_copy  = faad_get1bit(ld
        DEBUGVAR(1,112,"get_adif_header(): original_copy"));
    adif->home = faad_get1bit(ld
        DEBUGVAR(1,113,"get_adif_header(): home"));
    adif->bitstream_type = faad_get1bit(ld
        DEBUGVAR(1,114,"get_adif_header(): bitstream_type"));
    adif->bitrate = faad_getbits(ld, 23
        DEBUGVAR(1,115,"get_adif_header(): bitrate"));
    adif->num_program_config_elements = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,116,"get_adif_header(): num_program_config_elements"));

    for (i = 0; i < adif->num_program_config_elements + 1; i++)
    {
        if(adif->bitstream_type == 0)
        {
            adif->adif_buffer_fullness = faad_getbits(ld, 20
                DEBUGVAR(1,117,"get_adif_header(): adif_buffer_fullness"));
        } else {
            adif->adif_buffer_fullness = 0;
        }

        program_config_element(&adif->pce[i], ld);
    }
}
