ofputil_normalize_match__(struct match *match, bool may_log)
{
    enum {
        MAY_NW_ADDR     = 1 << 0, /* nw_src, nw_dst */
        MAY_TP_ADDR     = 1 << 1, /* tp_src, tp_dst */
        MAY_NW_PROTO    = 1 << 2, /* nw_proto */
        MAY_IPVx        = 1 << 3, /* tos, frag, ttl */
        MAY_ARP_SHA     = 1 << 4, /* arp_sha */
        MAY_ARP_THA     = 1 << 5, /* arp_tha */
        MAY_IPV6        = 1 << 6, /* ipv6_src, ipv6_dst, ipv6_label */
        MAY_ND_TARGET   = 1 << 7, /* nd_target */
        MAY_MPLS        = 1 << 8, /* mpls label and tc */
    } may_match;

    struct flow_wildcards wc;

    /* Figure out what fields may be matched. */
    if (match->flow.dl_type == htons(ETH_TYPE_IP)) {
        may_match = MAY_NW_PROTO | MAY_IPVx | MAY_NW_ADDR;
        if (match->flow.nw_proto == IPPROTO_TCP ||
            match->flow.nw_proto == IPPROTO_UDP ||
            match->flow.nw_proto == IPPROTO_SCTP ||
            match->flow.nw_proto == IPPROTO_ICMP) {
            may_match |= MAY_TP_ADDR;
        }
    } else if (match->flow.dl_type == htons(ETH_TYPE_IPV6)) {
        may_match = MAY_NW_PROTO | MAY_IPVx | MAY_IPV6;
        if (match->flow.nw_proto == IPPROTO_TCP ||
            match->flow.nw_proto == IPPROTO_UDP ||
            match->flow.nw_proto == IPPROTO_SCTP) {
            may_match |= MAY_TP_ADDR;
        } else if (match->flow.nw_proto == IPPROTO_ICMPV6) {
            may_match |= MAY_TP_ADDR;
            if (match->flow.tp_src == htons(ND_NEIGHBOR_SOLICIT)) {
                may_match |= MAY_ND_TARGET | MAY_ARP_SHA;
            } else if (match->flow.tp_src == htons(ND_NEIGHBOR_ADVERT)) {
                may_match |= MAY_ND_TARGET | MAY_ARP_THA;
            }
        }
    } else if (match->flow.dl_type == htons(ETH_TYPE_ARP) ||
               match->flow.dl_type == htons(ETH_TYPE_RARP)) {
        may_match = MAY_NW_PROTO | MAY_NW_ADDR | MAY_ARP_SHA | MAY_ARP_THA;
    } else if (eth_type_mpls(match->flow.dl_type)) {
        may_match = MAY_MPLS;
    } else {
        may_match = 0;
    }

    /* Clear the fields that may not be matched. */
    wc = match->wc;
    if (!(may_match & MAY_NW_ADDR)) {
        wc.masks.nw_src = wc.masks.nw_dst = htonl(0);
    }
    if (!(may_match & MAY_TP_ADDR)) {
        wc.masks.tp_src = wc.masks.tp_dst = htons(0);
    }
    if (!(may_match & MAY_NW_PROTO)) {
        wc.masks.nw_proto = 0;
    }
    if (!(may_match & MAY_IPVx)) {
        wc.masks.nw_tos = 0;
        wc.masks.nw_ttl = 0;
    }
    if (!(may_match & MAY_ARP_SHA)) {
        WC_UNMASK_FIELD(&wc, arp_sha);
    }
    if (!(may_match & MAY_ARP_THA)) {
        WC_UNMASK_FIELD(&wc, arp_tha);
    }
    if (!(may_match & MAY_IPV6)) {
        wc.masks.ipv6_src = wc.masks.ipv6_dst = in6addr_any;
        wc.masks.ipv6_label = htonl(0);
    }
    if (!(may_match & MAY_ND_TARGET)) {
        wc.masks.nd_target = in6addr_any;
    }
    if (!(may_match & MAY_MPLS)) {
        memset(wc.masks.mpls_lse, 0, sizeof wc.masks.mpls_lse);
    }

    /* Log any changes. */
    if (!flow_wildcards_equal(&wc, &match->wc)) {
        bool log = may_log && !VLOG_DROP_INFO(&bad_ofmsg_rl);
        char *pre = log ? match_to_string(match, OFP_DEFAULT_PRIORITY) : NULL;

        match->wc = wc;
        match_zero_wildcarded_fields(match);

        if (log) {
            char *post = match_to_string(match, OFP_DEFAULT_PRIORITY);
            VLOG_INFO("normalization changed ofp_match, details:");
            VLOG_INFO(" pre: %s", pre);
            VLOG_INFO("post: %s", post);
            free(pre);
            free(post);
        }
    }
}
