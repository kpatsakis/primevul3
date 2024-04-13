inline int PacketSetData(Packet *p, uint8_t *pktdata, uint32_t pktlen)
{
    SET_PKT_LEN(p, (size_t)pktlen);
    if (unlikely(!pktdata)) {
        return -1;
    }
    p->ext_pkt = pktdata;
    p->flags |= PKT_ZERO_COPY;

    return 0;
}
