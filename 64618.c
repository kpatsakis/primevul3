ofputil_port_stats_from_ofp13(struct ofputil_port_stats *ops,
                              const struct ofp13_port_stats *ps13)
{
    enum ofperr error = ofputil_port_stats_from_ofp11(ops, &ps13->ps);
    if (!error) {
        ops->duration_sec = ntohl(ps13->duration_sec);
        ops->duration_nsec = ntohl(ps13->duration_nsec);
    }
    return error;
}
