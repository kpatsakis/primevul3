inline int PacketCallocExtPkt(Packet *p, int datalen)
{
    if (! p->ext_pkt) {
        p->ext_pkt = SCCalloc(1, datalen);
        if (unlikely(p->ext_pkt == NULL)) {
            SET_PKT_LEN(p, 0);
            return -1;
        }
    }
    return 0;
}
