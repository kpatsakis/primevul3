static int get_consumed_bytes(MpegEncContext *s, int buf_size)
{
    int pos = (get_bits_count(&s->gb) + 7) >> 3;

    if (s->divx_packed || s->avctx->hwaccel) {
        /* We would have to scan through the whole buf to handle the weird
         * reordering ... */
        return buf_size;
    } else if (s->avctx->flags & AV_CODEC_FLAG_TRUNCATED) {
        pos -= s->parse_context.last_index;
        if (pos < 0)
            pos = 0;
        return pos;
    } else {
        if (pos == 0)
            pos = 1;
        if (pos + 10 > buf_size)
            pos = buf_size;

        return pos;
    }
}
