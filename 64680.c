ofputil_wildcard_from_ofpfw10(uint32_t ofpfw, struct flow_wildcards *wc)
{
    BUILD_ASSERT_DECL(FLOW_WC_SEQ == 36);

    /* Initialize most of wc. */
    flow_wildcards_init_catchall(wc);

    if (!(ofpfw & OFPFW10_IN_PORT)) {
        wc->masks.in_port.ofp_port = u16_to_ofp(UINT16_MAX);
    }

    if (!(ofpfw & OFPFW10_NW_TOS)) {
        wc->masks.nw_tos |= IP_DSCP_MASK;
    }

    if (!(ofpfw & OFPFW10_NW_PROTO)) {
        wc->masks.nw_proto = UINT8_MAX;
    }
    wc->masks.nw_src = ofputil_wcbits_to_netmask(ofpfw
                                                 >> OFPFW10_NW_SRC_SHIFT);
    wc->masks.nw_dst = ofputil_wcbits_to_netmask(ofpfw
                                                 >> OFPFW10_NW_DST_SHIFT);

    if (!(ofpfw & OFPFW10_TP_SRC)) {
        wc->masks.tp_src = OVS_BE16_MAX;
    }
    if (!(ofpfw & OFPFW10_TP_DST)) {
        wc->masks.tp_dst = OVS_BE16_MAX;
    }

    if (!(ofpfw & OFPFW10_DL_SRC)) {
        WC_MASK_FIELD(wc, dl_src);
    }
    if (!(ofpfw & OFPFW10_DL_DST)) {
        WC_MASK_FIELD(wc, dl_dst);
    }
    if (!(ofpfw & OFPFW10_DL_TYPE)) {
        wc->masks.dl_type = OVS_BE16_MAX;
    }

    /* VLAN TCI mask. */
    if (!(ofpfw & OFPFW10_DL_VLAN_PCP)) {
        wc->masks.vlan_tci |= htons(VLAN_PCP_MASK | VLAN_CFI);
    }
    if (!(ofpfw & OFPFW10_DL_VLAN)) {
        wc->masks.vlan_tci |= htons(VLAN_VID_MASK | VLAN_CFI);
    }
}
