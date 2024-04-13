    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_flow_mod fm;
    uint64_t ofpacts_stub[1024 / 8];
    struct ofpbuf ofpacts;
    enum ofperr error;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    ofpbuf_use_stub(&ofpacts, ofpacts_stub, sizeof ofpacts_stub);
    error = ofputil_decode_flow_mod(&fm, oh, ofconn_get_protocol(ofconn),
                                    ofproto_get_tun_tab(ofproto),
                                    &ofproto->vl_mff_map, &ofpacts,
                                    u16_to_ofp(ofproto->max_ports),
                                    ofproto->n_tables);
    if (!error) {
        struct openflow_mod_requester req = { ofconn, oh };
        error = handle_flow_mod__(ofproto, &fm, &req);
    }

    ofpbuf_uninit(&ofpacts);
    return error;
}
