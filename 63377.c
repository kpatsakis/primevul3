static int pack_streams (WavpackContext *wpc, uint32_t block_samples)
{
    uint32_t max_blocksize, max_chans = 1, bcount;
    unsigned char *outbuff, *outend, *out2buff, *out2end;
    int result = TRUE, i;


    for (i = 0; i < wpc->num_streams; i++)
        if (!(wpc->streams [i]->wphdr.flags & MONO_FLAG)) {
            max_chans = 2;
            break;
        }


    max_blocksize = block_samples * max_chans * ((wpc->streams [0]->wphdr.flags & BYTES_STORED) + 1);


    if ((wpc->config.flags & CONFIG_FLOAT_DATA) && !(wpc->config.flags & CONFIG_SKIP_WVX))
        max_blocksize += max_blocksize;         // 100% margin for lossless float data
    else
        max_blocksize += max_blocksize >> 2;    // otherwise 25% margin for everything else

    max_blocksize += wpc->metabytes + 1024;     // finally, add metadata & another 1K margin

    out2buff = (wpc->wvc_flag) ? malloc (max_blocksize) : NULL;
    out2end = out2buff + max_blocksize;
    outbuff = malloc (max_blocksize);
    outend = outbuff + max_blocksize;

    for (wpc->current_stream = 0; wpc->current_stream < wpc->num_streams; wpc->current_stream++) {
        WavpackStream *wps = wpc->streams [wpc->current_stream];
        uint32_t flags = wps->wphdr.flags;

        flags &= ~MAG_MASK;
        flags += (1 << MAG_LSB) * ((flags & BYTES_STORED) * 8 + 7);

        SET_BLOCK_INDEX (wps->wphdr, wps->sample_index);
        wps->wphdr.block_samples = block_samples;
        wps->wphdr.flags = flags;
        wps->block2buff = out2buff;
        wps->block2end = out2end;
        wps->blockbuff = outbuff;
        wps->blockend = outend;

#ifdef ENABLE_DSD
        if (flags & DSD_FLAG)
            result = pack_dsd_block (wpc, wps->sample_buffer);
        else
#endif
            result = pack_block (wpc, wps->sample_buffer);

        if (result) {
            result = block_add_checksum (outbuff, outend, (flags & HYBRID_FLAG) ? 2 : 4);

            if (result && out2buff)
                result = block_add_checksum (out2buff, out2end, 2);
        }

        wps->blockbuff = wps->block2buff = NULL;

        if (wps->wphdr.block_samples != block_samples)
            block_samples = wps->wphdr.block_samples;

        if (!result) {
            strcpy (wpc->error_message, "output buffer overflowed!");
            break;
        }

        bcount = ((WavpackHeader *) outbuff)->ckSize + 8;
        WavpackNativeToLittleEndian ((WavpackHeader *) outbuff, WavpackHeaderFormat);
        result = wpc->blockout (wpc->wv_out, outbuff, bcount);

        if (!result) {
            strcpy (wpc->error_message, "can't write WavPack data, disk probably full!");
            break;
        }

        wpc->filelen += bcount;

        if (out2buff) {
            bcount = ((WavpackHeader *) out2buff)->ckSize + 8;
            WavpackNativeToLittleEndian ((WavpackHeader *) out2buff, WavpackHeaderFormat);
            result = wpc->blockout (wpc->wvc_out, out2buff, bcount);

            if (!result) {
                strcpy (wpc->error_message, "can't write WavPack data, disk probably full!");
                break;
            }

            wpc->file2len += bcount;
        }

        if (wpc->acc_samples != block_samples)
            memmove (wps->sample_buffer, wps->sample_buffer + block_samples * (flags & MONO_FLAG ? 1 : 2),
                (wpc->acc_samples - block_samples) * sizeof (int32_t) * (flags & MONO_FLAG ? 1 : 2));
    }

    wpc->current_stream = 0;
    wpc->ave_block_samples = (wpc->ave_block_samples * 0x7 + block_samples + 0x4) >> 3;
    wpc->acc_samples -= block_samples;
    free (outbuff);

    if (out2buff)
        free (out2buff);

    return result;
}
