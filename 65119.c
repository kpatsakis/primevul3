static void flv_write_codec_header(AVFormatContext* s, AVCodecParameters* par, int64_t ts) {
    int64_t data_size;
    AVIOContext *pb = s->pb;
    FLVContext *flv = s->priv_data;

    if (par->codec_id == AV_CODEC_ID_AAC || par->codec_id == AV_CODEC_ID_H264
            || par->codec_id == AV_CODEC_ID_MPEG4) {
        int64_t pos;
        avio_w8(pb,
                par->codec_type == AVMEDIA_TYPE_VIDEO ?
                        FLV_TAG_TYPE_VIDEO : FLV_TAG_TYPE_AUDIO);
        avio_wb24(pb, 0); // size patched later
        put_timestamp(pb, ts);
        avio_wb24(pb, 0); // streamid
        pos = avio_tell(pb);
        if (par->codec_id == AV_CODEC_ID_AAC) {
            avio_w8(pb, get_audio_flags(s, par));
            avio_w8(pb, 0); // AAC sequence header

            if (!par->extradata_size && (flv->flags & FLV_AAC_SEQ_HEADER_DETECT)) {
                PutBitContext pbc;
                int samplerate_index;
                int channels = flv->audio_par->channels
                        - (flv->audio_par->channels == 8 ? 1 : 0);
                uint8_t data[2];

                for (samplerate_index = 0; samplerate_index < 16;
                        samplerate_index++)
                    if (flv->audio_par->sample_rate
                            == mpeg4audio_sample_rates[samplerate_index])
                        break;

                init_put_bits(&pbc, data, sizeof(data));
                put_bits(&pbc, 5, flv->audio_par->profile + 1); //profile
                put_bits(&pbc, 4, samplerate_index); //sample rate index
                put_bits(&pbc, 4, channels);
                put_bits(&pbc, 1, 0); //frame length - 1024 samples
                put_bits(&pbc, 1, 0); //does not depend on core coder
                put_bits(&pbc, 1, 0); //is not extension
                flush_put_bits(&pbc);

                avio_w8(pb, data[0]);
                avio_w8(pb, data[1]);

                av_log(s, AV_LOG_WARNING, "AAC sequence header: %02x %02x.\n",
                        data[0], data[1]);
            }
            avio_write(pb, par->extradata, par->extradata_size);
        } else {
            avio_w8(pb, par->codec_tag | FLV_FRAME_KEY); // flags
            avio_w8(pb, 0); // AVC sequence header
            avio_wb24(pb, 0); // composition time
            ff_isom_write_avcc(pb, par->extradata, par->extradata_size);
        }
        data_size = avio_tell(pb) - pos;
        avio_seek(pb, -data_size - 10, SEEK_CUR);
        avio_wb24(pb, data_size);
        avio_skip(pb, data_size + 10 - 3);
        avio_wb32(pb, data_size + 11); // previous tag size
    }
}
