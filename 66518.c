static int alloc_picture(MpegEncContext *s, Picture *pic, int shared)
{
    return ff_alloc_picture(s->avctx, pic, &s->me, &s->sc, shared, 0,
                            s->chroma_x_shift, s->chroma_y_shift, s->out_format,
                            s->mb_stride, s->mb_width, s->mb_height, s->b8_stride,
                            &s->linesize, &s->uvlinesize);
}
