static int flv_append_keyframe_info(AVFormatContext *s, FLVContext *flv, double ts, int64_t pos)
{
    FLVFileposition *position = av_malloc(sizeof(FLVFileposition));

    if (!position) {
        av_log(s, AV_LOG_WARNING, "no mem for add keyframe index!\n");
        return AVERROR(ENOMEM);
    }

    position->keyframe_timestamp = ts;
    position->keyframe_position = pos;

    if (!flv->filepositions_count) {
        flv->filepositions = position;
        flv->head_filepositions = flv->filepositions;
        position->next = NULL;
    } else {
        flv->filepositions->next = position;
        position->next = NULL;
        flv->filepositions = flv->filepositions->next;
    }

    flv->filepositions_count++;

    return 0;
}
