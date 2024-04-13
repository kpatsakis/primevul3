ofputil_queue_stats_to_ofp13(const struct ofputil_queue_stats *oqs,
                             struct ofp13_queue_stats *qs13)
{
    ofputil_queue_stats_to_ofp11(oqs, &qs13->qs);
    if (oqs->duration_sec != UINT32_MAX) {
        qs13->duration_sec = htonl(oqs->duration_sec);
        qs13->duration_nsec = htonl(oqs->duration_nsec);
    } else {
        qs13->duration_sec = OVS_BE32_MAX;
        qs13->duration_nsec = OVS_BE32_MAX;
    }
}
