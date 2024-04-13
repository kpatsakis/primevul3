ofputil_port_from_ofp11(ovs_be32 ofp11_port, ofp_port_t *ofp10_port)
{
    uint32_t ofp11_port_h = ntohl(ofp11_port);

    if (ofp11_port_h < ofp_to_u16(OFPP_MAX)) {
        *ofp10_port = u16_to_ofp(ofp11_port_h);
        return 0;
    } else if (ofp11_port_h >= ofp11_to_u32(OFPP11_MAX)) {
        *ofp10_port = u16_to_ofp(ofp11_port_h - OFPP11_OFFSET);
        return 0;
    } else {
        *ofp10_port = OFPP_NONE;
        VLOG_WARN_RL(&bad_ofmsg_rl, "port %"PRIu32" is outside the supported "
                     "range 0 through %d or 0x%"PRIx32" through 0x%"PRIx32,
                     ofp11_port_h, ofp_to_u16(OFPP_MAX) - 1,
                     ofp11_to_u32(OFPP11_MAX), UINT32_MAX);
        return OFPERR_OFPBAC_BAD_OUT_PORT;
    }
}
