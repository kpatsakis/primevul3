static int create_riff_header (WavpackContext *wpc, int64_t total_samples, void *outbuffer)
{
    int do_rf64 = 0, write_junk = 1;
    ChunkHeader ds64hdr, datahdr, fmthdr;
    char *outptr = outbuffer;
    RiffChunkHeader riffhdr;
    DS64Chunk ds64_chunk;
    JunkChunk junkchunk;
    WaveHeader wavhdr;

    int64_t total_data_bytes, total_riff_bytes;
    int32_t channel_mask = wpc->config.channel_mask;
    int32_t sample_rate = wpc->config.sample_rate;
    int bytes_per_sample = wpc->config.bytes_per_sample;
    int bits_per_sample = wpc->config.bits_per_sample;
    int format = (wpc->config.float_norm_exp) ? 3 : 1;
    int num_channels = wpc->config.num_channels;
    int wavhdrsize = 16;

    wpc->riff_header_created = TRUE;

    if (format == 3 && wpc->config.float_norm_exp != 127) {
        strcpy (wpc->error_message, "can't create valid RIFF wav header for non-normalized floating data!");
        return FALSE;
    }

    if (total_samples == -1)
        total_samples = 0x7ffff000 / (bytes_per_sample * num_channels);

    total_data_bytes = total_samples * bytes_per_sample * num_channels;

    if (total_data_bytes > 0xff000000) {
        write_junk = 0;
        do_rf64 = 1;
    }

    CLEAR (wavhdr);

    wavhdr.FormatTag = format;
    wavhdr.NumChannels = num_channels;
    wavhdr.SampleRate = sample_rate;
    wavhdr.BytesPerSecond = sample_rate * num_channels * bytes_per_sample;
    wavhdr.BlockAlign = bytes_per_sample * num_channels;
    wavhdr.BitsPerSample = bits_per_sample;

    if (num_channels > 2 || channel_mask != 0x5 - num_channels) {
        wavhdrsize = sizeof (wavhdr);
        wavhdr.cbSize = 22;
        wavhdr.ValidBitsPerSample = bits_per_sample;
        wavhdr.SubFormat = format;
        wavhdr.ChannelMask = channel_mask;
        wavhdr.FormatTag = 0xfffe;
        wavhdr.BitsPerSample = bytes_per_sample * 8;
        wavhdr.GUID [4] = 0x10;
        wavhdr.GUID [6] = 0x80;
        wavhdr.GUID [9] = 0xaa;
        wavhdr.GUID [11] = 0x38;
        wavhdr.GUID [12] = 0x9b;
        wavhdr.GUID [13] = 0x71;
    }

    strncpy (riffhdr.ckID, do_rf64 ? "RF64" : "RIFF", sizeof (riffhdr.ckID));
    strncpy (riffhdr.formType, "WAVE", sizeof (riffhdr.formType));
    total_riff_bytes = sizeof (riffhdr) + wavhdrsize + sizeof (datahdr) + total_data_bytes + wpc->riff_trailer_bytes;
    if (do_rf64) total_riff_bytes += sizeof (ds64hdr) + sizeof (ds64_chunk);
    if (write_junk) total_riff_bytes += sizeof (junkchunk);
    strncpy (fmthdr.ckID, "fmt ", sizeof (fmthdr.ckID));
    strncpy (datahdr.ckID, "data", sizeof (datahdr.ckID));
    fmthdr.ckSize = wavhdrsize;

    if (write_junk) {
        CLEAR (junkchunk);
        strncpy (junkchunk.ckID, "junk", sizeof (junkchunk.ckID));
        junkchunk.ckSize = sizeof (junkchunk) - 8;
        WavpackNativeToLittleEndian (&junkchunk, ChunkHeaderFormat);
    }

    if (do_rf64) {
        strncpy (ds64hdr.ckID, "ds64", sizeof (ds64hdr.ckID));
        ds64hdr.ckSize = sizeof (ds64_chunk);
        CLEAR (ds64_chunk);
        ds64_chunk.riffSize64 = total_riff_bytes;
        ds64_chunk.dataSize64 = total_data_bytes;
        ds64_chunk.sampleCount64 = total_samples;
        riffhdr.ckSize = (uint32_t) -1;
        datahdr.ckSize = (uint32_t) -1;
        WavpackNativeToLittleEndian (&ds64hdr, ChunkHeaderFormat);
        WavpackNativeToLittleEndian (&ds64_chunk, DS64ChunkFormat);
    }
    else {
        riffhdr.ckSize = (uint32_t) total_riff_bytes;
        datahdr.ckSize = (uint32_t) total_data_bytes;
    }

    WavpackNativeToLittleEndian (&riffhdr, ChunkHeaderFormat);
    WavpackNativeToLittleEndian (&fmthdr, ChunkHeaderFormat);
    WavpackNativeToLittleEndian (&wavhdr, WaveHeaderFormat);
    WavpackNativeToLittleEndian (&datahdr, ChunkHeaderFormat);


    outptr = (char *) memcpy (outptr, &riffhdr, sizeof (riffhdr)) + sizeof (riffhdr);

    if (do_rf64) {
        outptr = (char *) memcpy (outptr, &ds64hdr, sizeof (ds64hdr)) + sizeof (ds64hdr);
        outptr = (char *) memcpy (outptr, &ds64_chunk, sizeof (ds64_chunk)) + sizeof (ds64_chunk);
    }

    if (write_junk)
        outptr = (char *) memcpy (outptr, &junkchunk, sizeof (junkchunk)) + sizeof (junkchunk);

    outptr = (char *) memcpy (outptr, &fmthdr, sizeof (fmthdr)) + sizeof (fmthdr);
    outptr = (char *) memcpy (outptr, &wavhdr, wavhdrsize) + wavhdrsize;
    outptr = (char *) memcpy (outptr, &datahdr, sizeof (datahdr)) + sizeof (datahdr);

    return (int)(outptr - (char *) outbuffer);
}
