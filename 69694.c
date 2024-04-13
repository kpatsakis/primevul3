void PacketUpdateEngineEventCounters(ThreadVars *tv,
        DecodeThreadVars *dtv, Packet *p)
{
    for (uint8_t i = 0; i < p->events.cnt; i++) {
        const uint8_t e = p->events.events[i];

        if (e <= DECODE_EVENT_PACKET_MAX && !stats_decoder_events)
            continue;
        if (e > DECODE_EVENT_PACKET_MAX && !stats_stream_events)
            continue;
        StatsIncr(tv, dtv->counter_engine_events[e]);
    }
}
