ofputil_match_from_ofp10_match(const struct ofp10_match *ofmatch,
                               struct match *match)
{
    uint32_t ofpfw = ntohl(ofmatch->wildcards) & OFPFW10_ALL;

    /* Initialize match->wc. */
    memset(&match->flow, 0, sizeof match->flow);
    ofputil_wildcard_from_ofpfw10(ofpfw, &match->wc);
    memset(&match->tun_md, 0, sizeof match->tun_md);

    /* Initialize most of match->flow. */
    match->flow.nw_src = ofmatch->nw_src;
    match->flow.nw_dst = ofmatch->nw_dst;
    match->flow.in_port.ofp_port = u16_to_ofp(ntohs(ofmatch->in_port));
    match->flow.dl_type = ofputil_dl_type_from_openflow(ofmatch->dl_type);
    match->flow.tp_src = ofmatch->tp_src;
    match->flow.tp_dst = ofmatch->tp_dst;
    match->flow.dl_src = ofmatch->dl_src;
    match->flow.dl_dst = ofmatch->dl_dst;
    match->flow.nw_tos = ofmatch->nw_tos & IP_DSCP_MASK;
    match->flow.nw_proto = ofmatch->nw_proto;

    /* Translate VLANs. */
    if (!(ofpfw & OFPFW10_DL_VLAN) &&
        ofmatch->dl_vlan == htons(OFP10_VLAN_NONE)) {
        /* Match only packets without 802.1Q header.
         *
         * When OFPFW10_DL_VLAN_PCP is wildcarded, this is obviously correct.
         *
         * If OFPFW10_DL_VLAN_PCP is matched, the flow match is contradictory,
         * because we can't have a specific PCP without an 802.1Q header.
         * However, older versions of OVS treated this as matching packets
         * withut an 802.1Q header, so we do here too. */
        match->flow.vlan_tci = htons(0);
        match->wc.masks.vlan_tci = htons(0xffff);
    } else {
        ovs_be16 vid, pcp, tci;
        uint16_t hpcp;

        vid = ofmatch->dl_vlan & htons(VLAN_VID_MASK);
        hpcp = (ofmatch->dl_vlan_pcp << VLAN_PCP_SHIFT) & VLAN_PCP_MASK;
        pcp = htons(hpcp);
        tci = vid | pcp | htons(VLAN_CFI);
        match->flow.vlan_tci = tci & match->wc.masks.vlan_tci;
    }

    /* Clean up. */
    match_zero_wildcarded_fields(match);
}
