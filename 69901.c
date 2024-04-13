static int vqa_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
    VqaContext *s = avctx->priv_data;
    AVFrame *frame = data;
    int res;

    if ((res = ff_get_buffer(avctx, frame, 0)) < 0)
        return res;

    bytestream2_init(&s->gb, avpkt->data, avpkt->size);
    if ((res = vqa_decode_chunk(s, frame)) < 0)
        return res;

    /* make the palette available on the way out */
    memcpy(frame->data[1], s->palette, PALETTE_COUNT * 4);
    frame->palette_has_changed = 1;

    *got_frame      = 1;

    /* report that the buffer was completely consumed */
    return avpkt->size;
}
