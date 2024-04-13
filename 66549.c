static void gray_frame(AVFrame *frame)
{
    int i, h_chroma_shift, v_chroma_shift;

    av_pix_fmt_get_chroma_sub_sample(frame->format, &h_chroma_shift, &v_chroma_shift);

    for(i=0; i<frame->height; i++)
        memset(frame->data[0] + frame->linesize[0]*i, 0x80, frame->width);
    for(i=0; i<AV_CEIL_RSHIFT(frame->height, v_chroma_shift); i++) {
        memset(frame->data[1] + frame->linesize[1]*i,
               0x80, AV_CEIL_RSHIFT(frame->width, h_chroma_shift));
        memset(frame->data[2] + frame->linesize[2]*i,
               0x80, AV_CEIL_RSHIFT(frame->width, h_chroma_shift));
    }
}
