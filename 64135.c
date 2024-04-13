    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *p = ofconn_get_ofproto(ofconn);
    struct ofputil_packet_out po;
    struct ofproto_packet_out opo;
    uint64_t ofpacts_stub[1024 / 8];
    struct ofpbuf ofpacts;
    enum ofperr error;

    COVERAGE_INC(ofproto_packet_out);

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    /* Decode message. */
    ofpbuf_use_stub(&ofpacts, ofpacts_stub, sizeof ofpacts_stub);
    error = ofputil_decode_packet_out(&po, oh, &ofpacts);
    if (error) {
        ofpbuf_uninit(&ofpacts);
        return error;
    }

    po.ofpacts = ofpbuf_steal_data(&ofpacts);   /* Move to heap. */
    error = ofproto_packet_out_init(p, ofconn, &opo, &po);
    if (error) {
        free(po.ofpacts);
        return error;
    }

    ovs_mutex_lock(&ofproto_mutex);
    opo.version = p->tables_version;
    error = ofproto_packet_out_start(p, &opo);
    if (!error) {
        ofproto_packet_out_finish(p, &opo);
    }
    ovs_mutex_unlock(&ofproto_mutex);

    ofproto_packet_out_uninit(&opo);
    return error;
}
