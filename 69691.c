Packet *PacketGetFromAlloc(void)
{
    Packet *p = SCMalloc(SIZE_OF_PACKET);
    if (unlikely(p == NULL)) {
        return NULL;
    }

    memset(p, 0, SIZE_OF_PACKET);
    PACKET_INITIALIZE(p);
    p->ReleasePacket = PacketFree;
    p->flags |= PKT_ALLOC;

    SCLogDebug("allocated a new packet only using alloc...");

    PACKET_PROFILING_START(p);
    return p;
}
