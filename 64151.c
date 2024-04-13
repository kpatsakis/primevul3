    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    enum ofperr error;
    struct ofputil_bundle_add_msg badd;
    struct ofp_bundle_entry *bmsg;
    enum ofptype type;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    error = ofputil_decode_bundle_add(oh, &badd, &type);
    if (error) {
        return error;
    }

    bmsg = ofp_bundle_entry_alloc(type, badd.msg);

    struct ofpbuf ofpacts;
    uint64_t ofpacts_stub[1024 / 8];
    ofpbuf_use_stub(&ofpacts, ofpacts_stub, sizeof ofpacts_stub);

    if (type == OFPTYPE_PORT_MOD) {
        error = ofputil_decode_port_mod(badd.msg, &bmsg->opm.pm, false);
    } else if (type == OFPTYPE_FLOW_MOD) {
        struct ofputil_flow_mod fm;

        error = ofputil_decode_flow_mod(&fm, badd.msg,
                                        ofconn_get_protocol(ofconn),
                                        ofproto_get_tun_tab(ofproto),
                                        &ofproto->vl_mff_map, &ofpacts,
                                        u16_to_ofp(ofproto->max_ports),
                                        ofproto->n_tables);
        if (!error) {
            error = ofproto_flow_mod_init(ofproto, &bmsg->ofm, &fm, NULL);
        }
    } else if (type == OFPTYPE_GROUP_MOD) {
        error = ofputil_decode_group_mod(badd.msg, &bmsg->ogm.gm);
    } else if (type == OFPTYPE_PACKET_OUT) {
        struct ofputil_packet_out po;

        COVERAGE_INC(ofproto_packet_out);

        /* Decode message. */
        error = ofputil_decode_packet_out(&po, badd.msg, &ofpacts);
        if (!error) {
            po.ofpacts = ofpbuf_steal_data(&ofpacts);   /* Move to heap. */
            error = ofproto_packet_out_init(ofproto, ofconn, &bmsg->opo, &po);
        }
    } else {
        OVS_NOT_REACHED();
    }

    ofpbuf_uninit(&ofpacts);

    if (!error) {
        error = ofp_bundle_add_message(ofconn, badd.bundle_id, badd.flags,
                                       bmsg, oh);
    }

    if (error) {
        ofp_bundle_entry_free(bmsg);
    }

    return error;
}
