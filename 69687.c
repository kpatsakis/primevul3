void PacketDecodeFinalize(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p)
{
    if (p->flags & PKT_IS_INVALID) {
        StatsIncr(tv, dtv->counter_invalid);
    }
}
