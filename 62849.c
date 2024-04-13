static void swap_guid(ff_asf_guid guid)
{
    FFSWAP(unsigned char, guid[0], guid[3]);
    FFSWAP(unsigned char, guid[1], guid[2]);
    FFSWAP(unsigned char, guid[4], guid[5]);
    FFSWAP(unsigned char, guid[6], guid[7]);
}
