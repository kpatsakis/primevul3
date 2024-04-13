parse_port_mod_ethernet_property(struct ofpbuf *property,
                                 struct ofputil_port_mod *pm)
{
    ovs_be32 advertise;
    enum ofperr error;

    error = ofpprop_parse_be32(property, &advertise);
    if (!error) {
        pm->advertise = netdev_port_features_from_ofp11(advertise);
    }
    return error;
}
