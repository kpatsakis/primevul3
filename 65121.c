static int flv_write_trailer(AVFormatContext *s)
{
    int64_t file_size;
    AVIOContext *pb = s->pb;
    FLVContext *flv = s->priv_data;
    int build_keyframes_idx = flv->flags & FLV_ADD_KEYFRAME_INDEX;
    int i, res;
    int64_t cur_pos = avio_tell(s->pb);

    if (build_keyframes_idx) {
        FLVFileposition *newflv_posinfo, *p;

        avio_seek(pb, flv->videosize_offset, SEEK_SET);
        put_amf_double(pb, flv->videosize);

        avio_seek(pb, flv->audiosize_offset, SEEK_SET);
        put_amf_double(pb, flv->audiosize);

        avio_seek(pb, flv->lasttimestamp_offset, SEEK_SET);
        put_amf_double(pb, flv->lasttimestamp);

        avio_seek(pb, flv->lastkeyframetimestamp_offset, SEEK_SET);
        put_amf_double(pb, flv->lastkeyframetimestamp);

        avio_seek(pb, flv->lastkeyframelocation_offset, SEEK_SET);
        put_amf_double(pb, flv->lastkeyframelocation + flv->keyframe_index_size);
        avio_seek(pb, cur_pos, SEEK_SET);

        res = shift_data(s);
        if (res < 0) {
             goto end;
        }
        avio_seek(pb, flv->keyframes_info_offset, SEEK_SET);
        put_amf_string(pb, "filepositions");
        put_amf_dword_array(pb, flv->filepositions_count);
        for (newflv_posinfo = flv->head_filepositions; newflv_posinfo; newflv_posinfo = newflv_posinfo->next) {
            put_amf_double(pb, newflv_posinfo->keyframe_position + flv->keyframe_index_size);
        }

        put_amf_string(pb, "times");
        put_amf_dword_array(pb, flv->filepositions_count);
        for (newflv_posinfo = flv->head_filepositions; newflv_posinfo; newflv_posinfo = newflv_posinfo->next) {
            put_amf_double(pb, newflv_posinfo->keyframe_timestamp);
        }

        newflv_posinfo = flv->head_filepositions;
        while (newflv_posinfo) {
            p = newflv_posinfo->next;
            if (p) {
                newflv_posinfo->next = p->next;
                av_free(p);
                p = NULL;
            } else {
                av_free(newflv_posinfo);
                newflv_posinfo = NULL;
            }
        }

        put_amf_string(pb, "");
        avio_w8(pb, AMF_END_OF_OBJECT);

        avio_seek(pb, cur_pos + flv->keyframe_index_size, SEEK_SET);
    }

end:
    if (flv->flags & FLV_NO_SEQUENCE_END) {
        av_log(s, AV_LOG_DEBUG, "FLV no sequence end mode open\n");
    } else {
        /* Add EOS tag */
        for (i = 0; i < s->nb_streams; i++) {
            AVCodecParameters *par = s->streams[i]->codecpar;
            FLVStreamContext *sc = s->streams[i]->priv_data;
            if (par->codec_type == AVMEDIA_TYPE_VIDEO &&
                    (par->codec_id == AV_CODEC_ID_H264 || par->codec_id == AV_CODEC_ID_MPEG4))
                put_avc_eos_tag(pb, sc->last_ts);
        }
    }

    file_size = avio_tell(pb);

    if (build_keyframes_idx) {
        flv->datasize = file_size - flv->datastart_offset;
        avio_seek(pb, flv->datasize_offset, SEEK_SET);
        put_amf_double(pb, flv->datasize);
    }
    if (!(flv->flags & FLV_NO_METADATA)) {
        if (!(flv->flags & FLV_NO_DURATION_FILESIZE)) {
            /* update information */
            if (avio_seek(pb, flv->duration_offset, SEEK_SET) < 0) {
                av_log(s, AV_LOG_WARNING, "Failed to update header with correct duration.\n");
            } else {
                put_amf_double(pb, flv->duration / (double)1000);
            }
            if (avio_seek(pb, flv->filesize_offset, SEEK_SET) < 0) {
                av_log(s, AV_LOG_WARNING, "Failed to update header with correct filesize.\n");
            } else {
                put_amf_double(pb, file_size);
            }
        }
    }

    return 0;
}
