int WavpackAddWrapper (WavpackContext *wpc, void *data, uint32_t bcount)
{
    int64_t index = WavpackGetSampleIndex64 (wpc);
    unsigned char meta_id;

    if (!index || index == -1) {
        wpc->riff_header_added = TRUE;
        meta_id = wpc->file_format ? ID_ALT_HEADER : ID_RIFF_HEADER;
    }
    else {
        wpc->riff_trailer_bytes += bcount;
        meta_id = wpc->file_format ? ID_ALT_TRAILER : ID_RIFF_TRAILER;
    }

    return add_to_metadata (wpc, data, bcount, meta_id);
}
