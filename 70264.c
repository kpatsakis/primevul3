static uint16_t data_stream_element(NeAACDecStruct *hDecoder, bitfile *ld)
{
    uint8_t byte_aligned;
    uint16_t i, count;

    /* element_instance_tag = */ faad_getbits(ld, LEN_TAG
        DEBUGVAR(1,60,"data_stream_element(): element_instance_tag"));
    byte_aligned = faad_get1bit(ld
        DEBUGVAR(1,61,"data_stream_element(): byte_aligned"));
    count = (uint16_t)faad_getbits(ld, 8
        DEBUGVAR(1,62,"data_stream_element(): count"));
    if (count == 255)
    {
        count += (uint16_t)faad_getbits(ld, 8
            DEBUGVAR(1,63,"data_stream_element(): extra count"));
    }
    if (byte_aligned)
        faad_byte_align(ld);

    for (i = 0; i < count; i++)
    {
        faad_getbits(ld, LEN_BYTE
            DEBUGVAR(1,64,"data_stream_element(): data_stream_byte"));
    }

    return count;
}
