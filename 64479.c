ofputil_decode_group_mod(const struct ofp_header *oh,
                         struct ofputil_group_mod *gm)
{
    ofputil_init_group_properties(&gm->props);

    enum ofp_version ofp_version = oh->version;
    struct ofpbuf msg = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&msg);

    enum ofperr err;
    switch (ofp_version)
    {
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
        err = ofputil_pull_ofp11_group_mod(&msg, ofp_version, gm);
        break;

    case OFP15_VERSION:
    case OFP16_VERSION:
        err = ofputil_pull_ofp15_group_mod(&msg, ofp_version, gm);
        break;

    case OFP10_VERSION:
    default:
        OVS_NOT_REACHED();
    }
    if (err) {
        return err;
    }

    err = ofputil_check_group_mod(gm);
    if (err) {
        ofputil_uninit_group_mod(gm);
    }
    return err;
}
