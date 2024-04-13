ofputil_encode_meter_features_reply(const struct ofputil_meter_features *mf,
                                    const struct ofp_header *request)
{
    struct ofpbuf *reply;
    struct ofp13_meter_features *omf;

    reply = ofpraw_alloc_stats_reply(request, 0);
    omf = ofpbuf_put_zeros(reply, sizeof *omf);

    omf->max_meter = htonl(mf->max_meters);
    omf->band_types = htonl(mf->band_types);
    omf->capabilities = htonl(mf->capabilities);
    omf->max_bands = mf->max_bands;
    omf->max_color = mf->max_color;

    return reply;
}
