int check_codec_match(LayeredAVStream *ccf, AVStream *ccs, int stream)
{
    int matches = 1;

/* FIXME: Missed check on AVCodecContext.flags */
#define CHECK_CODEC(x)  (ccf->codecpar->x != ccs->codecpar->x)
    if (CHECK_CODEC(codec_id) || CHECK_CODEC(codec_type)) {
        http_log("Codecs do not match for stream %d\n", stream);
        matches = 0;
    } else if (CHECK_CODEC(bit_rate)) {
        http_log("Codec bitrates do not match for stream %d\n", stream);
        matches = 0;
    } else if (ccf->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        if (av_cmp_q(ccf->time_base, ccs->time_base) ||
            CHECK_CODEC(width) || CHECK_CODEC(height)) {
            http_log("Codec width, height or framerate do not match for stream %d\n", stream);
            matches = 0;
        }
    } else if (ccf->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
        if (CHECK_CODEC(sample_rate) ||
            CHECK_CODEC(channels) ||
            CHECK_CODEC(frame_size)) {
            http_log("Codec sample_rate, channels, frame_size do not match for stream %d\n", stream);
            matches = 0;
        }
    } else {
        http_log("Unknown codec type for stream %d\n", stream);
        matches = 0;
    }

    return matches;
}
