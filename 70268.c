static uint16_t extension_payload(bitfile *ld, drc_info *drc, uint16_t count)
{
    uint16_t i, n, dataElementLength;
    uint8_t dataElementLengthPart;
    uint8_t align = 4, data_element_version, loopCounter;

    uint8_t extension_type = (uint8_t)faad_getbits(ld, 4
        DEBUGVAR(1,87,"extension_payload(): extension_type"));

    switch (extension_type)
    {
    case EXT_DYNAMIC_RANGE:
        drc->present = 1;
        n = dynamic_range_info(ld, drc);
        return n;
    case EXT_FILL_DATA:
        /* fill_nibble = */ faad_getbits(ld, 4
            DEBUGVAR(1,136,"extension_payload(): fill_nibble")); /* must be 0000 */
        for (i = 0; i < count-1; i++)
        {
            /* fill_byte[i] = */ faad_getbits(ld, 8
                DEBUGVAR(1,88,"extension_payload(): fill_byte")); /* must be 10100101 */
        }
        return count;
    case EXT_DATA_ELEMENT:
        data_element_version = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,400,"extension_payload(): data_element_version"));
        switch (data_element_version)
        {
        case ANC_DATA:
            loopCounter = 0;
            dataElementLength = 0;
            do {
                dataElementLengthPart = (uint8_t)faad_getbits(ld, 8
                    DEBUGVAR(1,401,"extension_payload(): dataElementLengthPart"));
                dataElementLength += dataElementLengthPart;
                loopCounter++;
            } while (dataElementLengthPart == 255);

            for (i = 0; i < dataElementLength; i++)
            {
                /* data_element_byte[i] = */ faad_getbits(ld, 8
                    DEBUGVAR(1,402,"extension_payload(): data_element_byte"));
                return (dataElementLength+loopCounter+1);
            }
        default:
            align = 0;
        }
    case EXT_FIL:
    default:
        faad_getbits(ld, align
            DEBUGVAR(1,88,"extension_payload(): fill_nibble"));
        for (i = 0; i < count-1; i++)
        {
            /* other_bits[i] = */ faad_getbits(ld, 8
               DEBUGVAR(1,89,"extension_payload(): fill_bit"));
        }
        return count;
    }
}
