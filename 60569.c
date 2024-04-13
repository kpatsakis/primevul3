static void fmt_bytecount(AVIOContext *pb, int64_t count)
{
    static const char suffix[] = " kMGTP";
    const char *s;

    for (s = suffix; count >= 100000 && s[1]; count /= 1000, s++);

    avio_printf(pb, "%"PRId64"%c", count, *s);
}
