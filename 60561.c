static char *ctime1(char *buf2, size_t buf_size)
{
    time_t ti;
    char *p;

    ti = time(NULL);
    p = ctime(&ti);
    if (!p || !*p) {
        *buf2 = '\0';
        return buf2;
    }
    av_strlcpy(buf2, p, buf_size);
    p = buf2 + strlen(buf2) - 1;
    if (*p == '\n')
        *p = '\0';
    return buf2;
}
