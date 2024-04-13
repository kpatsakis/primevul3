ofputil_port_stats_to_ofp13(const struct ofputil_port_stats *ops,
                            struct ofp13_port_stats *ps13)
{
    ofputil_port_stats_to_ofp11(ops, &ps13->ps);
    ps13->duration_sec = htonl(ops->duration_sec);
    ps13->duration_nsec = htonl(ops->duration_nsec);
}
