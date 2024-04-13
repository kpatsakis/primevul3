static int shift_data(AVFormatContext *s)
{
    int ret = 0;
    int n = 0;
    int64_t metadata_size = 0;
    FLVContext *flv = s->priv_data;
    int64_t pos, pos_end = avio_tell(s->pb);
    uint8_t *buf, *read_buf[2];
    int read_buf_id = 0;
    int read_size[2];
    AVIOContext *read_pb;

    metadata_size = flv->filepositions_count * 9 * 2 + 10; /* filepositions and times value */
    metadata_size += 2 + 13; /* filepositions String */
    metadata_size += 2 + 5; /* times String */
    metadata_size += 3; /* Object end */

    flv->keyframe_index_size = metadata_size;

    if (metadata_size < 0)
        return metadata_size;

    buf = av_malloc_array(metadata_size, 2);
    if (!buf) {
        return AVERROR(ENOMEM);
    }
    read_buf[0] = buf;
    read_buf[1] = buf + metadata_size;

    avio_seek(s->pb, flv->metadata_size_pos, SEEK_SET);
    avio_wb24(s->pb, flv->metadata_totalsize + metadata_size);

    avio_seek(s->pb, flv->metadata_totalsize_pos, SEEK_SET);
    avio_wb32(s->pb, flv->metadata_totalsize + 11 + metadata_size);
    avio_seek(s->pb, pos_end, SEEK_SET);

    /* Shift the data: the AVIO context of the output can only be used for
     * writing, so we re-open the same output, but for reading. It also avoids
     * a read/seek/write/seek back and forth. */
    avio_flush(s->pb);
    ret = s->io_open(s, &read_pb, s->url, AVIO_FLAG_READ, NULL);
    if (ret < 0) {
        av_log(s, AV_LOG_ERROR, "Unable to re-open %s output file for "
               "the second pass (add_keyframe_index)\n", s->url);
        goto end;
    }

    /* mark the end of the shift to up to the last data we wrote, and get ready
     * for writing */
    pos_end = avio_tell(s->pb);
    avio_seek(s->pb, flv->keyframes_info_offset + metadata_size, SEEK_SET);

    /* start reading at where the keyframe index information will be placed */
    avio_seek(read_pb, flv->keyframes_info_offset, SEEK_SET);
    pos = avio_tell(read_pb);

#define READ_BLOCK do {                                                             \
    read_size[read_buf_id] = avio_read(read_pb, read_buf[read_buf_id], metadata_size);  \
    read_buf_id ^= 1;                                                               \
} while (0)

    /* shift data by chunk of at most keyframe *filepositions* and *times* size */
    READ_BLOCK;
    do {
        READ_BLOCK;
        n = read_size[read_buf_id];
        if (n < 0)
            break;
        avio_write(s->pb, read_buf[read_buf_id], n);
        pos += n;
    } while (pos <= pos_end);

    ff_format_io_close(s, &read_pb);

end:
    av_free(buf);
    return ret;
}
