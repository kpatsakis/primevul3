ofputil_match_to_ofp10_match(const struct match *match,
                             struct ofp10_match *ofmatch)
{
    const struct flow_wildcards *wc = &match->wc;
    uint32_t ofpfw;

    /* Figure out most OpenFlow wildcards. */
    ofpfw = 0;
    if (!wc->masks.in_port.ofp_port) {
        ofpfw |= OFPFW10_IN_PORT;
    }
    if (!wc->masks.dl_type) {
        ofpfw |= OFPFW10_DL_TYPE;
    }
    if (!wc->masks.nw_proto) {
        ofpfw |= OFPFW10_NW_PROTO;
    }
    ofpfw |= (ofputil_netmask_to_wcbits(wc->masks.nw_src)
              << OFPFW10_NW_SRC_SHIFT);
    ofpfw |= (ofputil_netmask_to_wcbits(wc->masks.nw_dst)
              << OFPFW10_NW_DST_SHIFT);
    if (!(wc->masks.nw_tos & IP_DSCP_MASK)) {
        ofpfw |= OFPFW10_NW_TOS;
    }
    if (!wc->masks.tp_src) {
        ofpfw |= OFPFW10_TP_SRC;
    }
    if (!wc->masks.tp_dst) {
        ofpfw |= OFPFW10_TP_DST;
    }
    if (eth_addr_is_zero(wc->masks.dl_src)) {
        ofpfw |= OFPFW10_DL_SRC;
    }
    if (eth_addr_is_zero(wc->masks.dl_dst)) {
        ofpfw |= OFPFW10_DL_DST;
    }

    /* Translate VLANs. */
    ofmatch->dl_vlan = htons(0);
    ofmatch->dl_vlan_pcp = 0;
    if (match->wc.masks.vlan_tci == htons(0)) {
        ofpfw |= OFPFW10_DL_VLAN | OFPFW10_DL_VLAN_PCP;
    } else if (match->wc.masks.vlan_tci & htons(VLAN_CFI)
               && !(match->flow.vlan_tci & htons(VLAN_CFI))) {
        ofmatch->dl_vlan = htons(OFP10_VLAN_NONE);
    } else {
        if (!(match->wc.masks.vlan_tci & htons(VLAN_VID_MASK))) {
            ofpfw |= OFPFW10_DL_VLAN;
        } else {
            ofmatch->dl_vlan = htons(vlan_tci_to_vid(match->flow.vlan_tci));
        }

        if (!(match->wc.masks.vlan_tci & htons(VLAN_PCP_MASK))) {
            ofpfw |= OFPFW10_DL_VLAN_PCP;
        } else {
            ofmatch->dl_vlan_pcp = vlan_tci_to_pcp(match->flow.vlan_tci);
        }
    }

    /* Compose most of the match structure. */
    ofmatch->wildcards = htonl(ofpfw);
    ofmatch->in_port = htons(ofp_to_u16(match->flow.in_port.ofp_port));
    ofmatch->dl_src = match->flow.dl_src;
    ofmatch->dl_dst = match->flow.dl_dst;
    ofmatch->dl_type = ofputil_dl_type_to_openflow(match->flow.dl_type);
    ofmatch->nw_src = match->flow.nw_src;
    ofmatch->nw_dst = match->flow.nw_dst;
    ofmatch->nw_tos = match->flow.nw_tos & IP_DSCP_MASK;
    ofmatch->nw_proto = match->flow.nw_proto;
    ofmatch->tp_src = match->flow.tp_src;
    ofmatch->tp_dst = match->flow.tp_dst;
    memset(ofmatch->pad1, '\0', sizeof ofmatch->pad1);
    memset(ofmatch->pad2, '\0', sizeof ofmatch->pad2);
}
