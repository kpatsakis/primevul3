static int read_config_info (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    int bytecnt = wpmd->byte_length;
    unsigned char *byteptr = wpmd->data;

    if (bytecnt >= 3) {
        wpc->config.flags &= 0xff;
        wpc->config.flags |= (int32_t) *byteptr++ << 8;
        wpc->config.flags |= (int32_t) *byteptr++ << 16;
        wpc->config.flags |= (int32_t) *byteptr++ << 24;
        bytecnt -= 3;

        if (bytecnt && (wpc->config.flags & CONFIG_EXTRA_MODE)) {
            wpc->config.xmode = *byteptr++;
            bytecnt--;
        }


        if (bytecnt) {
            wpc->config.qmode = (wpc->config.qmode & ~0xff) | *byteptr;
            wpc->version_five = 1;
        }
    }

    return TRUE;
}
