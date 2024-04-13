static int pva_probe(AVProbeData * pd) {
    const unsigned char *buf = pd->buf;
    int len = pva_check(buf);

    if (len < 0)
        return 0;

    if (pd->buf_size >= len + 8 &&
        pva_check(buf + len) >= 0)
        return AVPROBE_SCORE_EXTENSION;

    return AVPROBE_SCORE_MAX / 4;
}
