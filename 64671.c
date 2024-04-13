ofputil_queue_stats_to_ofp14(const struct ofputil_queue_stats *oqs,
                             struct ofp14_queue_stats *qs14)
{
    qs14->length = htons(sizeof *qs14);
    memset(qs14->pad, 0, sizeof qs14->pad);
    ofputil_queue_stats_to_ofp13(oqs, &qs14->qs);
}
