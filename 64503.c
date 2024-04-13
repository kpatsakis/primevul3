ofputil_decode_port_mod(const struct ofp_header *oh,
                        struct ofputil_port_mod *pm, bool loose)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPT10_PORT_MOD) {
        const struct ofp10_port_mod *opm = b.data;

        pm->port_no = u16_to_ofp(ntohs(opm->port_no));
        pm->hw_addr = opm->hw_addr;
        pm->config = ntohl(opm->config) & OFPPC10_ALL;
        pm->mask = ntohl(opm->mask) & OFPPC10_ALL;
        pm->advertise = netdev_port_features_from_ofp10(opm->advertise);
    } else if (raw == OFPRAW_OFPT11_PORT_MOD) {
        const struct ofp11_port_mod *opm = b.data;
        enum ofperr error;

        error = ofputil_port_from_ofp11(opm->port_no, &pm->port_no);
        if (error) {
            return error;
        }

        pm->hw_addr = opm->hw_addr;
        pm->config = ntohl(opm->config) & OFPPC11_ALL;
        pm->mask = ntohl(opm->mask) & OFPPC11_ALL;
        pm->advertise = netdev_port_features_from_ofp11(opm->advertise);
    } else if (raw == OFPRAW_OFPT14_PORT_MOD) {
        const struct ofp14_port_mod *opm = ofpbuf_pull(&b, sizeof *opm);
        enum ofperr error;

        memset(pm, 0, sizeof *pm);

        error = ofputil_port_from_ofp11(opm->port_no, &pm->port_no);
        if (error) {
            return error;
        }

        pm->hw_addr = opm->hw_addr;
        pm->config = ntohl(opm->config) & OFPPC11_ALL;
        pm->mask = ntohl(opm->mask) & OFPPC11_ALL;

        while (b.size > 0) {
            struct ofpbuf property;
            enum ofperr error;
            uint64_t type;

            error = ofpprop_pull(&b, &property, &type);
            if (error) {
                return error;
            }

            switch (type) {
            case OFPPMPT14_ETHERNET:
                error = parse_port_mod_ethernet_property(&property, pm);
                break;

            default:
                error = OFPPROP_UNKNOWN(loose, "port_mod", type);
                break;
            }

            if (error) {
                return error;
            }
        }
    } else {
        return OFPERR_OFPBRC_BAD_TYPE;
    }

    pm->config &= pm->mask;
    return 0;
}
