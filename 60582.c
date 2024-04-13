static int http_start_receive_data(HTTPContext *c)
{
    int fd;
    int ret;
    int64_t ret64;

    if (c->stream->feed_opened) {
        http_log("Stream feed '%s' was not opened\n",
                 c->stream->feed_filename);
        return AVERROR(EINVAL);
    }

    /* Don't permit writing to this one */
    if (c->stream->readonly) {
        http_log("Cannot write to read-only file '%s'\n",
                 c->stream->feed_filename);
        return AVERROR(EINVAL);
    }

    /* open feed */
    fd = open(c->stream->feed_filename, O_RDWR);
    if (fd < 0) {
        ret = AVERROR(errno);
        http_log("Could not open feed file '%s': %s\n",
                 c->stream->feed_filename, strerror(errno));
        return ret;
    }
    c->feed_fd = fd;

    if (c->stream->truncate) {
        /* truncate feed file */
        ffm_write_write_index(c->feed_fd, FFM_PACKET_SIZE);
        http_log("Truncating feed file '%s'\n", c->stream->feed_filename);
        if (ftruncate(c->feed_fd, FFM_PACKET_SIZE) < 0) {
            ret = AVERROR(errno);
            http_log("Error truncating feed file '%s': %s\n",
                     c->stream->feed_filename, strerror(errno));
            return ret;
        }
    } else {
        ret64 = ffm_read_write_index(fd);
        if (ret64 < 0) {
            http_log("Error reading write index from feed file '%s': %s\n",
                     c->stream->feed_filename, strerror(errno));
            return ret64;
        }
        c->stream->feed_write_index = ret64;
    }

    c->stream->feed_write_index = FFMAX(ffm_read_write_index(fd),
                                        FFM_PACKET_SIZE);
    c->stream->feed_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    /* init buffer input */
    c->buffer_ptr = c->buffer;
    c->buffer_end = c->buffer + FFM_PACKET_SIZE;
    c->stream->feed_opened = 1;
    c->chunked_encoding = !!av_stristr(c->buffer, "Transfer-Encoding: chunked");
    return 0;
}
