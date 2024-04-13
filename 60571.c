static int64_t get_packet_send_clock(HTTPContext *c)
{
    int bytes_left, bytes_sent, frame_bytes;

    frame_bytes = c->cur_frame_bytes;
    if (frame_bytes <= 0)
        return c->cur_pts;

    bytes_left = c->buffer_end - c->buffer_ptr;
    bytes_sent = frame_bytes - bytes_left;
    return c->cur_pts + (c->cur_frame_duration * bytes_sent) / frame_bytes;
}
