void PacketFreeOrRelease(Packet *p)
{
    if (p->flags & PKT_ALLOC)
        PacketFree(p);
    else
        PacketPoolReturnPacket(p);
}
