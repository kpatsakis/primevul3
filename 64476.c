ofputil_decode_group_desc_reply(struct ofputil_group_desc *gd,
                                struct ofpbuf *msg, enum ofp_version version)
{
    ofputil_init_group_properties(&gd->props);

    switch (version)
    {
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
        return ofputil_decode_ofp11_group_desc_reply(gd, msg, version);

    case OFP15_VERSION:
    case OFP16_VERSION:
        return ofputil_decode_ofp15_group_desc_reply(gd, msg, version);

    case OFP10_VERSION:
    default:
        OVS_NOT_REACHED();
    }
}
