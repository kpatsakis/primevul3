    OVS_REQUIRES(ofproto_mutex)
{
    struct ofmonitor *m;
    enum ofperr error;

    m = ofmonitor_lookup(ofconn, id);
    if (m) {
        ofmonitor_destroy(m);
        error = 0;
    } else {
        error = OFPERR_OFPMOFC_UNKNOWN_MONITOR;
    }

    return error;
}
