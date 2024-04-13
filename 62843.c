static AVStream *find_stream(AVFormatContext *s, uint16_t st_num)
{
    AVStream *st = NULL;
    ASFContext *asf = s->priv_data;
    int i;

    for (i = 0; i < asf->nb_streams; i++) {
        if (asf->asf_st[i]->stream_index == st_num) {
            st = s->streams[asf->asf_st[i]->index];
            break;
        }
    }

    return st;
}
