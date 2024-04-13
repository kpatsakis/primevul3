int WavpackPackSamples (WavpackContext *wpc, int32_t *sample_buffer, uint32_t sample_count)
{
    int nch = wpc->config.num_channels;

    while (sample_count) {
        int32_t *source_pointer = sample_buffer;
        unsigned int samples_to_copy;

        if (!wpc->riff_header_added && !wpc->riff_header_created && !wpc->file_format) {
            char riff_header [128];

            if (!add_to_metadata (wpc, riff_header, create_riff_header (wpc, wpc->total_samples, riff_header), ID_RIFF_HEADER))
                return FALSE;
        }

        if (wpc->acc_samples + sample_count > wpc->max_samples)
            samples_to_copy = wpc->max_samples - wpc->acc_samples;
        else
            samples_to_copy = sample_count;

        for (wpc->current_stream = 0; wpc->current_stream < wpc->num_streams; wpc->current_stream++) {
            WavpackStream *wps = wpc->streams [wpc->current_stream];
            int32_t *dptr, *sptr, cnt;

            dptr = wps->sample_buffer + wpc->acc_samples * (wps->wphdr.flags & MONO_FLAG ? 1 : 2);
            sptr = source_pointer;
            cnt = samples_to_copy;


            if (wps->wphdr.flags & MONO_FLAG) {
                switch (wpc->config.bytes_per_sample) {
                    case 1:
                        while (cnt--) {
                            *dptr++ = (signed char) *sptr;
                            sptr += nch;
                        }

                        break;

                    case 2:
                        while (cnt--) {
                            *dptr++ = (int16_t) *sptr;
                            sptr += nch;
                        }

                        break;

                    case 3:
                        while (cnt--) {
                            *dptr++ = (*sptr << 8) >> 8;
                            sptr += nch;
                        }

                        break;

                    default:
                        while (cnt--) {
                            *dptr++ = *sptr;
                            sptr += nch;
                        }
                }

                source_pointer++;
            }
            else {
                switch (wpc->config.bytes_per_sample) {
                    case 1:
                        while (cnt--) {
                            *dptr++ = (signed char) sptr [0];
                            *dptr++ = (signed char) sptr [1];
                            sptr += nch;
                        }

                        break;

                    case 2:
                        while (cnt--) {
                            *dptr++ = (int16_t) sptr [0];
                            *dptr++ = (int16_t) sptr [1];
                            sptr += nch;
                        }

                        break;

                    case 3:
                        while (cnt--) {
                            *dptr++ = (sptr [0] << 8) >> 8;
                            *dptr++ = (sptr [1] << 8) >> 8;
                            sptr += nch;
                        }

                        break;

                    default:
                        while (cnt--) {
                            *dptr++ = sptr [0];
                            *dptr++ = sptr [1];
                            sptr += nch;
                        }
                }

                source_pointer += 2;
            }
        }

        sample_buffer += samples_to_copy * nch;
        sample_count -= samples_to_copy;

        if ((wpc->acc_samples += samples_to_copy) == wpc->max_samples &&
            !pack_streams (wpc, wpc->block_samples))
                return FALSE;
    }

    return TRUE;
}
