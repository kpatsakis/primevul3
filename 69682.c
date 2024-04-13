void DecodeRegisterPerfCounters(DecodeThreadVars *dtv, ThreadVars *tv)
{
    /* register counters */
    dtv->counter_pkts = StatsRegisterCounter("decoder.pkts", tv);
    dtv->counter_bytes = StatsRegisterCounter("decoder.bytes", tv);
    dtv->counter_invalid = StatsRegisterCounter("decoder.invalid", tv);
    dtv->counter_ipv4 = StatsRegisterCounter("decoder.ipv4", tv);
    dtv->counter_ipv6 = StatsRegisterCounter("decoder.ipv6", tv);
    dtv->counter_eth = StatsRegisterCounter("decoder.ethernet", tv);
    dtv->counter_raw = StatsRegisterCounter("decoder.raw", tv);
    dtv->counter_null = StatsRegisterCounter("decoder.null", tv);
    dtv->counter_sll = StatsRegisterCounter("decoder.sll", tv);
    dtv->counter_tcp = StatsRegisterCounter("decoder.tcp", tv);
    dtv->counter_udp = StatsRegisterCounter("decoder.udp", tv);
    dtv->counter_sctp = StatsRegisterCounter("decoder.sctp", tv);
    dtv->counter_icmpv4 = StatsRegisterCounter("decoder.icmpv4", tv);
    dtv->counter_icmpv6 = StatsRegisterCounter("decoder.icmpv6", tv);
    dtv->counter_ppp = StatsRegisterCounter("decoder.ppp", tv);
    dtv->counter_pppoe = StatsRegisterCounter("decoder.pppoe", tv);
    dtv->counter_gre = StatsRegisterCounter("decoder.gre", tv);
    dtv->counter_vlan = StatsRegisterCounter("decoder.vlan", tv);
    dtv->counter_vlan_qinq = StatsRegisterCounter("decoder.vlan_qinq", tv);
    dtv->counter_ieee8021ah = StatsRegisterCounter("decoder.ieee8021ah", tv);
    dtv->counter_teredo = StatsRegisterCounter("decoder.teredo", tv);
    dtv->counter_ipv4inipv6 = StatsRegisterCounter("decoder.ipv4_in_ipv6", tv);
    dtv->counter_ipv6inipv6 = StatsRegisterCounter("decoder.ipv6_in_ipv6", tv);
    dtv->counter_mpls = StatsRegisterCounter("decoder.mpls", tv);
    dtv->counter_avg_pkt_size = StatsRegisterAvgCounter("decoder.avg_pkt_size", tv);
    dtv->counter_max_pkt_size = StatsRegisterMaxCounter("decoder.max_pkt_size", tv);
    dtv->counter_erspan = StatsRegisterMaxCounter("decoder.erspan", tv);
    dtv->counter_flow_memcap = StatsRegisterCounter("flow.memcap", tv);

    dtv->counter_flow_tcp = StatsRegisterCounter("flow.tcp", tv);
    dtv->counter_flow_udp = StatsRegisterCounter("flow.udp", tv);
    dtv->counter_flow_icmp4 = StatsRegisterCounter("flow.icmpv4", tv);
    dtv->counter_flow_icmp6 = StatsRegisterCounter("flow.icmpv6", tv);

    dtv->counter_defrag_ipv4_fragments =
        StatsRegisterCounter("defrag.ipv4.fragments", tv);
    dtv->counter_defrag_ipv4_reassembled =
        StatsRegisterCounter("defrag.ipv4.reassembled", tv);
    dtv->counter_defrag_ipv4_timeouts =
        StatsRegisterCounter("defrag.ipv4.timeouts", tv);
    dtv->counter_defrag_ipv6_fragments =
        StatsRegisterCounter("defrag.ipv6.fragments", tv);
    dtv->counter_defrag_ipv6_reassembled =
        StatsRegisterCounter("defrag.ipv6.reassembled", tv);
    dtv->counter_defrag_ipv6_timeouts =
        StatsRegisterCounter("defrag.ipv6.timeouts", tv);
    dtv->counter_defrag_max_hit =
        StatsRegisterCounter("defrag.max_frag_hits", tv);

    for (int i = 0; i < DECODE_EVENT_MAX; i++) {
        BUG_ON(i != (int)DEvents[i].code);

        if (i <= DECODE_EVENT_PACKET_MAX && !stats_decoder_events)
            continue;
        if (i > DECODE_EVENT_PACKET_MAX && !stats_stream_events)
            continue;

        dtv->counter_engine_events[i] = StatsRegisterCounter(
                DEvents[i].event_name, tv);
    }

    return;
}
