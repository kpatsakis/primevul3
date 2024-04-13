void PacketDefragPktSetupParent(Packet *parent)
{
    /* tell parent packet it's part of a tunnel */
    SET_TUNNEL_PKT(parent);

    /* increment tunnel packet refcnt in the root packet */
    TUNNEL_INCR_PKT_TPR(parent);

    /* disable payload (not packet) inspection on the parent, as the payload
     * is the packet we will now run through the system separately. We do
     * check it against the ip/port/other header checks though */
    DecodeSetNoPayloadInspectionFlag(parent);
}
