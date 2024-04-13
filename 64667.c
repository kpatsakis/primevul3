ofputil_queue_stats_from_ofp13(struct ofputil_queue_stats *oqs,
                               const struct ofp13_queue_stats *qs13)
{
    enum ofperr error = ofputil_queue_stats_from_ofp11(oqs, &qs13->qs);
    if (!error) {
        oqs->duration_sec = ntohl(qs13->duration_sec);
        oqs->duration_nsec = ntohl(qs13->duration_nsec);
    }

    return error;
}
