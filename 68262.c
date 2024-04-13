static void StreamTcpPseudoPacketCreateDetectLogFlush(ThreadVars *tv,
        StreamTcpThread *stt, Packet *p, TcpSession *ssn, PacketQueue *pq, bool dir)
{
    SCEnter();

    if (p->flags & PKT_PSEUDO_DETECTLOG_FLUSH) {
        SCReturn;
    }

    Packet *np = StreamTcpPseudoSetup(p, GET_PKT_DATA(p), GET_PKT_LEN(p));
    if (np == NULL) {
        SCLogDebug("The packet received from packet allocation is NULL");
        StatsIncr(tv, stt->counter_tcp_pseudo_failed);
        SCReturn;
    }
    PKT_SET_SRC(np, PKT_SRC_STREAM_TCP_DETECTLOG_FLUSH);

    /* Setup the IP and TCP headers */
    StreamTcpPseudoPacketSetupHeader(np,p);

    np->tenant_id = p->flow->tenant_id;
    np->flowflags = p->flowflags;

    np->flags |= PKT_STREAM_EST;
    np->flags |= PKT_HAS_FLOW;
    np->flags |= PKT_IGNORE_CHECKSUM;
    np->flags |= PKT_PSEUDO_DETECTLOG_FLUSH;

    if (p->flags & PKT_NOPACKET_INSPECTION) {
        DecodeSetNoPacketInspectionFlag(np);
    }
    if (p->flags & PKT_NOPAYLOAD_INSPECTION) {
        DecodeSetNoPayloadInspectionFlag(np);
    }

    if (dir == false) {
        SCLogDebug("pseudo is to_client");
        np->flowflags &= ~(FLOW_PKT_TOSERVER|FLOW_PKT_TOCLIENT);
        np->flowflags |= FLOW_PKT_TOCLIENT;
#ifdef DEBUG
        BUG_ON(!(PKT_IS_TOCLIENT(np)));
        BUG_ON((PKT_IS_TOSERVER(np)));
#endif
    } else {
        SCLogDebug("pseudo is to_server");
        np->flowflags &= ~(FLOW_PKT_TOCLIENT|FLOW_PKT_TOSERVER);
        np->flowflags |= FLOW_PKT_TOSERVER;
#ifdef DEBUG
        BUG_ON(!(PKT_IS_TOSERVER(np)));
        BUG_ON((PKT_IS_TOCLIENT(np)));
#endif
    }

    SCLogDebug("np %p", np);
    PacketEnqueue(pq, np);

    StatsIncr(tv, stt->counter_tcp_pseudo);
    SCReturn;
}
