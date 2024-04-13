int WavpackSetConfiguration (WavpackContext *wpc, WavpackConfig *config, uint32_t total_samples)
{
    config->flags |= CONFIG_COMPATIBLE_WRITE;       // write earlier version streams

    if (total_samples == (uint32_t) -1)
        return WavpackSetConfiguration64 (wpc, config, -1, NULL);
    else
        return WavpackSetConfiguration64 (wpc, config, total_samples, NULL);
}
