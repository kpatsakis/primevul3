ofputil_switch_features_has_ports(struct ofpbuf *b)
{
    struct ofp_header *oh = b->data;
    size_t phy_port_size;

    if (oh->version >= OFP13_VERSION) {
        /* OpenFlow 1.3+ never has ports in the feature reply. */
        return false;
    }

    phy_port_size = (oh->version == OFP10_VERSION
                     ? sizeof(struct ofp10_phy_port)
                     : sizeof(struct ofp11_port));
    if (ntohs(oh->length) + phy_port_size <= UINT16_MAX) {
        /* There's room for additional ports in the feature reply.
         * Assume that the list is complete. */
        return true;
    }

    /* The feature reply has no room for more ports.  Probably the list is
     * truncated.  Drop the ports and tell the caller to retrieve them with
     * OFPST_PORT_DESC. */
    b->size = sizeof *oh + sizeof(struct ofp_switch_features);
    ofpmsg_update_length(b);
    return false;
}
