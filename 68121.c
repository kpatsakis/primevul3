int WriteCaffHeader (FILE *outfile, WavpackContext *wpc, int64_t total_samples, int qmode)
{
    CAFChunkHeader caf_desc_chunk_header, caf_chan_chunk_header, caf_data_chunk_header;
    CAFChannelLayout caf_channel_layout;
    CAFAudioFormat caf_audio_format;
    CAFFileHeader caf_file_header;
    uint32_t mEditCount, bcount;

    int num_channels = WavpackGetNumChannels (wpc);
    int32_t channel_mask = WavpackGetChannelMask (wpc);
    int32_t sample_rate = WavpackGetSampleRate (wpc);
    int bytes_per_sample = WavpackGetBytesPerSample (wpc);
    int bits_per_sample = WavpackGetBitsPerSample (wpc);
    int float_norm_exp = WavpackGetFloatNormExp (wpc);
    uint32_t channel_layout_tag = WavpackGetChannelLayout (wpc, NULL);
    unsigned char *channel_identities = malloc (num_channels + 1);
    int num_identified_chans, i;

    if (float_norm_exp && float_norm_exp != 127) {
        error_line ("can't create valid CAFF header for non-normalized floating data!");
        free (channel_identities);
        return FALSE;
    }


    WavpackGetChannelIdentities (wpc, channel_identities);

    for (num_identified_chans = i = 0; i < num_channels; ++i)
        if (channel_identities [i] != 0xff)
            num_identified_chans++;


    strncpy (caf_file_header.mFileType, "caff", sizeof (caf_file_header.mFileType));
    caf_file_header.mFileVersion = 1;
    caf_file_header.mFileFlags = 0;
    WavpackNativeToBigEndian (&caf_file_header, CAFFileHeaderFormat);

    if (!DoWriteFile (outfile, &caf_file_header, sizeof (caf_file_header), &bcount) ||
        bcount != sizeof (caf_file_header))
            return FALSE;


    strncpy (caf_desc_chunk_header.mChunkType, "desc", sizeof (caf_desc_chunk_header.mChunkType));
    caf_desc_chunk_header.mChunkSize = sizeof (caf_audio_format);
    WavpackNativeToBigEndian (&caf_desc_chunk_header, CAFChunkHeaderFormat);

    if (!DoWriteFile (outfile, &caf_desc_chunk_header, sizeof (caf_desc_chunk_header), &bcount) ||
        bcount != sizeof (caf_desc_chunk_header))
            return FALSE;

    caf_audio_format.mSampleRate = (double) sample_rate;
    strncpy (caf_audio_format.mFormatID, "lpcm", sizeof (caf_audio_format.mFormatID));
    caf_audio_format.mFormatFlags = float_norm_exp ? CAF_FORMAT_FLOAT : 0;

    if (!(qmode & QMODE_BIG_ENDIAN))
        caf_audio_format.mFormatFlags |= CAF_FORMAT_LITTLE_ENDIAN;

    caf_audio_format.mBytesPerPacket = bytes_per_sample * num_channels;
    caf_audio_format.mFramesPerPacket = 1;
    caf_audio_format.mChannelsPerFrame = num_channels;
    caf_audio_format.mBitsPerChannel = bits_per_sample;
    WavpackNativeToBigEndian (&caf_audio_format, CAFAudioFormatFormat);

    if (!DoWriteFile (outfile, &caf_audio_format, sizeof (caf_audio_format), &bcount) ||
        bcount != sizeof (caf_audio_format))
            return FALSE;


    if (channel_layout_tag || (num_channels > 2 ? num_identified_chans : channel_mask != 5 - num_channels)) {
        int bits = 0, bmask;

        for (bmask = 1; bmask; bmask <<= 1)     // count the set bits in the channel mask
            if (bmask & channel_mask)
                ++bits;


        if ((channel_layout_tag & 0xff0000) || (bits == num_channels && !(qmode & QMODE_REORDERED_CHANS))) {

            strncpy (caf_chan_chunk_header.mChunkType, "chan", sizeof (caf_chan_chunk_header.mChunkType));
            caf_chan_chunk_header.mChunkSize = sizeof (caf_channel_layout);
            WavpackNativeToBigEndian (&caf_chan_chunk_header, CAFChunkHeaderFormat);

            if (!DoWriteFile (outfile, &caf_chan_chunk_header, sizeof (caf_chan_chunk_header), &bcount) ||
                bcount != sizeof (caf_chan_chunk_header))
                    return FALSE;

            if (channel_layout_tag) {
                if (debug_logging_mode)
                    error_line ("writing \"chan\" chunk with layout tag 0x%08x", channel_layout_tag);

                caf_channel_layout.mChannelLayoutTag = channel_layout_tag;
                caf_channel_layout.mChannelBitmap = 0;
            }
            else {
                if (debug_logging_mode)
                    error_line ("writing \"chan\" chunk with UseChannelBitmap tag, bitmap = 0x%08x", channel_mask);

                caf_channel_layout.mChannelLayoutTag = kCAFChannelLayoutTag_UseChannelBitmap;
                caf_channel_layout.mChannelBitmap = channel_mask;
            }

            caf_channel_layout.mNumberChannelDescriptions = 0;
            WavpackNativeToBigEndian (&caf_channel_layout, CAFChannelLayoutFormat);

            if (!DoWriteFile (outfile, &caf_channel_layout, sizeof (caf_channel_layout), &bcount) ||
                bcount != sizeof (caf_channel_layout))
                    return FALSE;
        }
        else {  // write a channel description array because a single layout or bitmap won't do it...
            CAFChannelDescription caf_channel_description;
            unsigned char *new_channel_order = NULL;
            int i;

            if (debug_logging_mode)
                error_line ("writing \"chan\" chunk with UseChannelDescriptions tag, bitmap = 0x%08x, reordered = %s",
                    channel_mask, (qmode & QMODE_REORDERED_CHANS) ? "yes" : "no");

            if (qmode & QMODE_REORDERED_CHANS) {
                if ((int)(channel_layout_tag & 0xff) <= num_channels) {
                    new_channel_order = malloc (num_channels);

                    for (i = 0; i < num_channels; ++i)
                        new_channel_order [i] = i;

                    WavpackGetChannelLayout (wpc, new_channel_order);
                }
            }

            strncpy (caf_chan_chunk_header.mChunkType, "chan", sizeof (caf_chan_chunk_header.mChunkType));
            caf_chan_chunk_header.mChunkSize = sizeof (caf_channel_layout) + sizeof (caf_channel_description) * num_channels;
            WavpackNativeToBigEndian (&caf_chan_chunk_header, CAFChunkHeaderFormat);

            if (!DoWriteFile (outfile, &caf_chan_chunk_header, sizeof (caf_chan_chunk_header), &bcount) ||
                bcount != sizeof (caf_chan_chunk_header))
                    return FALSE;

            caf_channel_layout.mChannelLayoutTag = kCAFChannelLayoutTag_UseChannelDescriptions;
            caf_channel_layout.mChannelBitmap = 0;
            caf_channel_layout.mNumberChannelDescriptions = num_channels;
            WavpackNativeToBigEndian (&caf_channel_layout, CAFChannelLayoutFormat);

            if (!DoWriteFile (outfile, &caf_channel_layout, sizeof (caf_channel_layout), &bcount) ||
                bcount != sizeof (caf_channel_layout))
                    return FALSE;

            for (i = 0; i < num_channels; ++i) {
                unsigned char chan_id = new_channel_order ? channel_identities [new_channel_order [i]] : channel_identities [i];
                CLEAR (caf_channel_description);

                if ((chan_id >= 1 && chan_id <= 18) || (chan_id >= 33 && chan_id <= 44) || (chan_id >= 200 && chan_id <= 207))
                    caf_channel_description.mChannelLabel = chan_id;
                else if (chan_id >= 221 && chan_id <= 225)
                    caf_channel_description.mChannelLabel = chan_id + 80;

                if (debug_logging_mode)
                    error_line ("chan %d --> %d", i + 1, caf_channel_description.mChannelLabel);

                WavpackNativeToBigEndian (&caf_channel_description, CAFChannelDescriptionFormat);

                if (!DoWriteFile (outfile, &caf_channel_description, sizeof (caf_channel_description), &bcount) ||
                    bcount != sizeof (caf_channel_description))
                        return FALSE;
            }

            if (new_channel_order)
                free (new_channel_order);
        }
    }


    strncpy (caf_data_chunk_header.mChunkType, "data", sizeof (caf_data_chunk_header.mChunkType));

    if (total_samples == -1)
        caf_data_chunk_header.mChunkSize = -1;
    else
        caf_data_chunk_header.mChunkSize = (total_samples * bytes_per_sample * num_channels) + sizeof (mEditCount);

    WavpackNativeToBigEndian (&caf_data_chunk_header, CAFChunkHeaderFormat);

    if (!DoWriteFile (outfile, &caf_data_chunk_header, sizeof (caf_data_chunk_header), &bcount) ||
        bcount != sizeof (caf_data_chunk_header))
            return FALSE;

    mEditCount = 0;
    WavpackNativeToBigEndian (&mEditCount, "L");

    if (!DoWriteFile (outfile, &mEditCount, sizeof (mEditCount), &bcount) ||
        bcount != sizeof (mEditCount))
            return FALSE;

    free (channel_identities);

    return TRUE;
}
