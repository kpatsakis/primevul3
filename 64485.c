ofputil_decode_meter_features(const struct ofp_header *oh,
                              struct ofputil_meter_features *mf)
{
    const struct ofp13_meter_features *omf = ofpmsg_body(oh);

    mf->max_meters = ntohl(omf->max_meter);
    mf->band_types = ntohl(omf->band_types);
    mf->capabilities = ntohl(omf->capabilities);
    mf->max_bands = omf->max_bands;
    mf->max_color = omf->max_color;
}
