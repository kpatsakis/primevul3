static int aa_read_packet(AVFormatContext *s, AVPacket *pkt)
{
    uint8_t dst[TEA_BLOCK_SIZE];
    uint8_t src[TEA_BLOCK_SIZE];
    int i;
    int trailing_bytes;
    int blocks;
    uint8_t buf[MAX_CODEC_SECOND_SIZE * 2];
    int written = 0;
    int ret;
    AADemuxContext *c = s->priv_data;
    uint64_t pos = avio_tell(s->pb);

    if (pos >= c->content_end) {
        return AVERROR_EOF;
    }

    if (c->current_chapter_size == 0) {
        c->current_chapter_size = avio_rb32(s->pb);
        if (c->current_chapter_size == 0) {
            return AVERROR_EOF;
        }
        av_log(s, AV_LOG_DEBUG, "Chapter %d (%" PRId64 " bytes)\n", c->chapter_idx, c->current_chapter_size);
        c->chapter_idx = c->chapter_idx + 1;
        avio_skip(s->pb, 4); // data start offset
        pos += 8;
        c->current_codec_second_size = c->codec_second_size;
    }

    if (c->current_chapter_size / c->current_codec_second_size == 0) {
        c->current_codec_second_size = c->current_chapter_size % c->current_codec_second_size;
    }

    blocks = c->current_codec_second_size / TEA_BLOCK_SIZE;
    for (i = 0; i < blocks; i++) {
        ret = avio_read(s->pb, src, TEA_BLOCK_SIZE);
        if (ret != TEA_BLOCK_SIZE)
            return (ret < 0) ? ret : AVERROR_EOF;
        av_tea_init(c->tea_ctx, c->file_key, 16);
        av_tea_crypt(c->tea_ctx, dst, src, 1, NULL, 1);
        memcpy(buf + written, dst, TEA_BLOCK_SIZE);
        written = written + TEA_BLOCK_SIZE;
    }
    trailing_bytes = c->current_codec_second_size % TEA_BLOCK_SIZE;
    if (trailing_bytes != 0) { // trailing bytes are left unencrypted!
        ret = avio_read(s->pb, src, trailing_bytes);
        if (ret != trailing_bytes)
            return (ret < 0) ? ret : AVERROR_EOF;
        memcpy(buf + written, src, trailing_bytes);
        written = written + trailing_bytes;
    }

    c->current_chapter_size = c->current_chapter_size - c->current_codec_second_size;
    if (c->current_chapter_size <= 0)
        c->current_chapter_size = 0;

    if (c->seek_offset > written)
        c->seek_offset = 0; // ignore wrong estimate

    ret = av_new_packet(pkt, written - c->seek_offset);
    if (ret < 0)
        return ret;
    memcpy(pkt->data, buf + c->seek_offset, written - c->seek_offset);
    pkt->pos = pos;

    c->seek_offset = 0;
    return 0;
}
