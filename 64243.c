handle_meter_request(struct ofconn *ofconn, const struct ofp_header *request,
                     enum ofptype type)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ovs_list replies;
    uint64_t bands_stub[256 / 8];
    struct ofpbuf bands;
    uint32_t meter_id, first, last;

    ofputil_decode_meter_request(request, &meter_id);

    if (meter_id == OFPM13_ALL) {
        first = 1;
        last = ofproto->meter_features.max_meters;
    } else {
        if (!meter_id || meter_id > ofproto->meter_features.max_meters ||
            !ofproto->meters[meter_id]) {
            return OFPERR_OFPMMFC_UNKNOWN_METER;
        }
        first = last = meter_id;
    }

    ofpbuf_use_stub(&bands, bands_stub, sizeof bands_stub);
    ofpmp_init(&replies, request);

    for (meter_id = first; meter_id <= last; ++meter_id) {
        struct meter *meter = ofproto->meters[meter_id];
        if (!meter) {
            continue; /* Skip non-existing meters. */
        }
        if (type == OFPTYPE_METER_STATS_REQUEST) {
            struct ofputil_meter_stats stats;

            stats.meter_id = meter_id;

            /* Provider sets the packet and byte counts, we do the rest. */
            stats.flow_count = ovs_list_size(&meter->rules);
            calc_duration(meter->created, time_msec(),
                          &stats.duration_sec, &stats.duration_nsec);
            stats.n_bands = meter->n_bands;
            ofpbuf_clear(&bands);
            stats.bands
                = ofpbuf_put_uninit(&bands,
                                    meter->n_bands * sizeof *stats.bands);

            if (!ofproto->ofproto_class->meter_get(ofproto,
                                                   meter->provider_meter_id,
                                                   &stats)) {
                ofputil_append_meter_stats(&replies, &stats);
            }
        } else { /* type == OFPTYPE_METER_CONFIG_REQUEST */
            struct ofputil_meter_config config;

            config.meter_id = meter_id;
            config.flags = meter->flags;
            config.n_bands = meter->n_bands;
            config.bands = meter->bands;
            ofputil_append_meter_config(&replies, &config);
        }
    }

    ofconn_send_replies(ofconn, &replies);
    ofpbuf_uninit(&bands);
    return 0;
}
