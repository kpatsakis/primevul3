WavpackContext *WavpackOpenFileOutput (WavpackBlockOutput blockout, void *wv_id, void *wvc_id)
{
    WavpackContext *wpc = malloc (sizeof (WavpackContext));

    if (!wpc)
        return NULL;

    CLEAR (*wpc);
    wpc->total_samples = -1;
    wpc->stream_version = CUR_STREAM_VERS;
    wpc->blockout = blockout;
    wpc->wv_out = wv_id;
    wpc->wvc_out = wvc_id;
    return wpc;
}
