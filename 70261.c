static void adts_variable_header(adts_header *adts, bitfile *ld)
{
    adts->copyright_identification_bit = faad_get1bit(ld
        DEBUGVAR(1,129,"adts_variable_header(): copyright_identification_bit"));
    adts->copyright_identification_start = faad_get1bit(ld
        DEBUGVAR(1,130,"adts_variable_header(): copyright_identification_start"));
    adts->aac_frame_length = (uint16_t)faad_getbits(ld, 13
        DEBUGVAR(1,131,"adts_variable_header(): aac_frame_length"));
    adts->adts_buffer_fullness = (uint16_t)faad_getbits(ld, 11
        DEBUGVAR(1,132,"adts_variable_header(): adts_buffer_fullness"));
    adts->no_raw_data_blocks_in_frame = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,133,"adts_variable_header(): no_raw_data_blocks_in_frame"));
}
