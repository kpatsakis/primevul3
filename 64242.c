handle_meter_mod(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_meter_mod mm;
    uint64_t bands_stub[256 / 8];
    struct ofpbuf bands;
    uint32_t meter_id;
    enum ofperr error;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    ofpbuf_use_stub(&bands, bands_stub, sizeof bands_stub);

    error = ofputil_decode_meter_mod(oh, &mm, &bands);
    if (error) {
        goto exit_free_bands;
    }

    meter_id = mm.meter.meter_id;

    if (mm.command != OFPMC13_DELETE) {
        /* Fails also when meters are not implemented by the provider. */
        if (meter_id == 0 || meter_id > OFPM13_MAX) {
            error = OFPERR_OFPMMFC_INVALID_METER;
            goto exit_free_bands;
        } else if (meter_id > ofproto->meter_features.max_meters) {
            error = OFPERR_OFPMMFC_OUT_OF_METERS;
            goto exit_free_bands;
        }
        if (mm.meter.n_bands > ofproto->meter_features.max_bands) {
            error = OFPERR_OFPMMFC_OUT_OF_BANDS;
            goto exit_free_bands;
        }
    }

    switch (mm.command) {
    case OFPMC13_ADD:
        error = handle_add_meter(ofproto, &mm);
        break;

    case OFPMC13_MODIFY:
        error = handle_modify_meter(ofproto, &mm);
        break;

    case OFPMC13_DELETE:
        error = handle_delete_meter(ofconn, &mm);
        break;

    default:
        error = OFPERR_OFPMMFC_BAD_COMMAND;
        break;
    }

    if (!error) {
        struct ofputil_requestforward rf;
        rf.xid = oh->xid;
        rf.reason = OFPRFR_METER_MOD;
        rf.meter_mod = &mm;
        connmgr_send_requestforward(ofproto->connmgr, ofconn, &rf);
    }

exit_free_bands:
    ofpbuf_uninit(&bands);
    return error;
}
