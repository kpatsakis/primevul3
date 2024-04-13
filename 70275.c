static uint32_t latmParsePayload(latm_header *latm, bitfile *ld)
{
    uint32_t framelen;
    uint8_t tmp;

    framelen = 0;
    if(latm->framelen_type==0)
    {
        do
        {
            tmp = (uint8_t)faad_getbits(ld, 8);
            framelen += tmp;
        } while(tmp==0xff);
    }
    else if(latm->framelen_type==1)
        framelen=latm->frameLength;

    return framelen;
}
