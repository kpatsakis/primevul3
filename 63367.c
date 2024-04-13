int WavpackSetChannelLayout (WavpackContext *wpc, uint32_t layout_tag, const unsigned char *reorder)
{
    int nchans = layout_tag & 0xff;

    if ((layout_tag & 0xff00ff00) || nchans > wpc->config.num_channels)
        return FALSE;

    wpc->channel_layout = layout_tag;

    if (wpc->channel_reordering) {
        free (wpc->channel_reordering);
        wpc->channel_reordering = NULL;
    }

    if (nchans && reorder) {
        int min_index = 256, i;

        for (i = 0; i < nchans; ++i)
            if (reorder [i] < min_index)
                min_index = reorder [i];

        wpc->channel_reordering = malloc (nchans);

        if (wpc->channel_reordering)
            for (i = 0; i < nchans; ++i)
                wpc->channel_reordering [i] = reorder [i] - min_index;
    }

    return TRUE;
}
