ofproto_packet_out_uninit(struct ofproto_packet_out *opo)
{
    dp_packet_delete(opo->packet);
    opo->packet = NULL;
    free(opo->flow);
    opo->flow = NULL;
    free(opo->ofpacts);
    opo->ofpacts = NULL;
    opo->ofpacts_len = 0;
    ovs_assert(!opo->aux);
}
