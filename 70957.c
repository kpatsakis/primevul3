static int aa_read_close(AVFormatContext *s)
{
    AADemuxContext *c = s->priv_data;

    av_freep(&c->tea_ctx);

    return 0;
}
