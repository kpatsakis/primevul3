static int asf_probe(AVProbeData *pd)
{
    /* check file header */
    if (!ff_guidcmp(pd->buf, &ff_asf_header))
        return AVPROBE_SCORE_MAX/2;
    else
        return 0;
}
