ofputil_put_phy_port(enum ofp_version ofp_version,
                     const struct ofputil_phy_port *pp, struct ofpbuf *b)
{
    switch (ofp_version) {
    case OFP10_VERSION: {
        struct ofp10_phy_port *opp = ofpbuf_put_uninit(b, sizeof *opp);
        ofputil_encode_ofp10_phy_port(pp, opp);
        break;
    }

    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION: {
        struct ofp11_port *op = ofpbuf_put_uninit(b, sizeof *op);
        ofputil_encode_ofp11_port(pp, op);
        break;
    }

    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        ofputil_put_ofp14_port(pp, b);
        break;

    default:
        OVS_NOT_REACHED();
    }
}
