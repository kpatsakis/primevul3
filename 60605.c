static void unlayer_stream(AVStream *st, LayeredAVStream *lst)
{
    avcodec_free_context(&st->codec);
    avcodec_parameters_free(&st->codecpar);
#define COPY(a) st->a = lst->a;
    COPY(index)
    COPY(id)
    COPY(codec)
    COPY(codecpar)
    COPY(time_base)
    COPY(pts_wrap_bits)
    COPY(sample_aspect_ratio)
    COPY(recommended_encoder_configuration)
}
