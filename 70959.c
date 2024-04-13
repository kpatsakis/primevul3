static int aa_read_seek(AVFormatContext *s,
                        int stream_index, int64_t timestamp, int flags)
{
    AADemuxContext *c = s->priv_data;
    AVChapter *ch;
    int64_t chapter_pos, chapter_start, chapter_size;
    int chapter_idx = 0;

    if (timestamp < 0)
        timestamp = 0;

    while (chapter_idx < s->nb_chapters && timestamp >= s->chapters[chapter_idx]->end) {
        ++chapter_idx;
    }

    if (chapter_idx >= s->nb_chapters) {
        chapter_idx = s->nb_chapters - 1;
        if (chapter_idx < 0) return -1; // there is no chapter.
        timestamp = s->chapters[chapter_idx]->end;
    }

    ch = s->chapters[chapter_idx];

    chapter_size = ch->end / TIMEPREC - ch->start / TIMEPREC;
    chapter_pos = av_rescale_rnd((timestamp - ch->start) / TIMEPREC,
        1, c->codec_second_size,
        (flags & AVSEEK_FLAG_BACKWARD) ? AV_ROUND_DOWN : AV_ROUND_UP)
        * c->codec_second_size;
    if (chapter_pos >= chapter_size)
        chapter_pos = chapter_size;
    chapter_start = c->content_start + (ch->start / TIMEPREC) + CHAPTER_HEADER_SIZE * (1 + chapter_idx);

    avio_seek(s->pb, chapter_start + chapter_pos, SEEK_SET);
    c->current_codec_second_size = c->codec_second_size;
    c->current_chapter_size = chapter_size - chapter_pos;
    c->chapter_idx = 1 + chapter_idx;

    if (s->streams[0]->codecpar->codec_id == AV_CODEC_ID_MP3) {
        c->seek_offset = (MP3_FRAME_SIZE - chapter_pos % MP3_FRAME_SIZE) % MP3_FRAME_SIZE;
    }

    ff_update_cur_dts(s, s->streams[0], ch->start + (chapter_pos + c->seek_offset) * TIMEPREC);

    return 1;
}
