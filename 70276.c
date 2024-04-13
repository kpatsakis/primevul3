static uint32_t latm_get_value(bitfile *ld)
{
    uint32_t l, value;
    uint8_t bytesForValue;

    bytesForValue = (uint8_t)faad_getbits(ld, 2);
    value = 0;
    for(l=0; l<bytesForValue; l++)
        value = (value << 8) | (uint8_t)faad_getbits(ld, 8);

    return value;
}
