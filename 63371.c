void WavpackUpdateNumSamples (WavpackContext *wpc, void *first_block)
{
    uint32_t wrapper_size;

    WavpackLittleEndianToNative (first_block, WavpackHeaderFormat);
    SET_TOTAL_SAMPLES (* (WavpackHeader *) first_block, WavpackGetSampleIndex64 (wpc));

    if (wpc->riff_header_created && WavpackGetWrapperLocation (first_block, &wrapper_size)) {
        unsigned char riff_header [128];

        if (wrapper_size == create_riff_header (wpc, WavpackGetSampleIndex64 (wpc), riff_header))
            memcpy (WavpackGetWrapperLocation (first_block, NULL), riff_header, wrapper_size);
    }

    block_update_checksum (first_block);
    WavpackNativeToLittleEndian (first_block, WavpackHeaderFormat);
}
