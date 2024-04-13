ofputil_encode_port_mod(const struct ofputil_port_mod *pm,
                        enum ofputil_protocol protocol)
{
    enum ofp_version ofp_version = ofputil_protocol_to_ofp_version(protocol);
    struct ofpbuf *b;

    switch (ofp_version) {
    case OFP10_VERSION: {
        struct ofp10_port_mod *opm;

        b = ofpraw_alloc(OFPRAW_OFPT10_PORT_MOD, ofp_version, 0);
        opm = ofpbuf_put_zeros(b, sizeof *opm);
        opm->port_no = htons(ofp_to_u16(pm->port_no));
        opm->hw_addr = pm->hw_addr;
        opm->config = htonl(pm->config & OFPPC10_ALL);
        opm->mask = htonl(pm->mask & OFPPC10_ALL);
        opm->advertise = netdev_port_features_to_ofp10(pm->advertise);
        break;
    }

    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION: {
        struct ofp11_port_mod *opm;

        b = ofpraw_alloc(OFPRAW_OFPT11_PORT_MOD, ofp_version, 0);
        opm = ofpbuf_put_zeros(b, sizeof *opm);
        opm->port_no = ofputil_port_to_ofp11(pm->port_no);
        opm->hw_addr = pm->hw_addr;
        opm->config = htonl(pm->config & OFPPC11_ALL);
        opm->mask = htonl(pm->mask & OFPPC11_ALL);
        opm->advertise = netdev_port_features_to_ofp11(pm->advertise);
        break;
    }
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION: {
        struct ofp14_port_mod *opm;

        b = ofpraw_alloc(OFPRAW_OFPT14_PORT_MOD, ofp_version, 0);
        opm = ofpbuf_put_zeros(b, sizeof *opm);
        opm->port_no = ofputil_port_to_ofp11(pm->port_no);
        opm->hw_addr = pm->hw_addr;
        opm->config = htonl(pm->config & OFPPC11_ALL);
        opm->mask = htonl(pm->mask & OFPPC11_ALL);

        if (pm->advertise) {
            ofpprop_put_be32(b, OFPPMPT14_ETHERNET,
                             netdev_port_features_to_ofp11(pm->advertise));
        }
        break;
    }
    default:
        OVS_NOT_REACHED();
    }

    return b;
}
