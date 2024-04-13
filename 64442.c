ofputil_append_meter_stats(struct ovs_list *replies,
                           const struct ofputil_meter_stats *ms)
{
    struct ofp13_meter_stats *reply;
    uint16_t n = 0;
    uint16_t len;

    len = sizeof *reply + ms->n_bands * sizeof(struct ofp13_meter_band_stats);
    reply = ofpmp_append(replies, len);

    reply->meter_id = htonl(ms->meter_id);
    reply->len = htons(len);
    memset(reply->pad, 0, sizeof reply->pad);
    reply->flow_count = htonl(ms->flow_count);
    reply->packet_in_count = htonll(ms->packet_in_count);
    reply->byte_in_count = htonll(ms->byte_in_count);
    reply->duration_sec = htonl(ms->duration_sec);
    reply->duration_nsec = htonl(ms->duration_nsec);

    for (n = 0; n < ms->n_bands; ++n) {
        const struct ofputil_meter_band_stats *src = &ms->bands[n];
        struct ofp13_meter_band_stats *dst = &reply->band_stats[n];

        dst->packet_band_count = htonll(src->packet_count);
        dst->byte_band_count = htonll(src->byte_count);
    }
}
