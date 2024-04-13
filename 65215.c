static void lzh_free(struct kwajd_stream *lzh)
{
    struct mspack_system *sys;
    if (!lzh || !lzh->sys) return;
    sys = lzh->sys;
    sys->free(lzh);
}
