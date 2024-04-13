ofputil_pull_phy_port(enum ofp_version ofp_version, struct ofpbuf *b,
                      struct ofputil_phy_port *pp)
{
    memset(pp, 0, sizeof *pp);

    switch (ofp_version) {
    case OFP10_VERSION: {
        const struct ofp10_phy_port *opp = ofpbuf_try_pull(b, sizeof *opp);
        return opp ? ofputil_decode_ofp10_phy_port(pp, opp) : EOF;
    }
    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION: {
        const struct ofp11_port *op = ofpbuf_try_pull(b, sizeof *op);
        return op ? ofputil_decode_ofp11_port(pp, op) : EOF;
    }
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        return b->size ? ofputil_pull_ofp14_port(pp, b) : EOF;
    default:
        OVS_NOT_REACHED();
    }
}
