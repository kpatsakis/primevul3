static int aa_probe(const AVProbeData *p)
{
    uint8_t *buf = p->buf;

    if (AV_RB32(buf+4) != AA_MAGIC)
        return 0;

    return AVPROBE_SCORE_MAX / 2;
}
