static av_cold int vqa_decode_end(AVCodecContext *avctx)
{
    VqaContext *s = avctx->priv_data;

    av_freep(&s->codebook);
    av_freep(&s->next_codebook_buffer);
    av_freep(&s->decode_buffer);

    return 0;
}
