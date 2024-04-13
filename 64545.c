ofputil_encode_meter_mod(enum ofp_version ofp_version,
                         const struct ofputil_meter_mod *mm)
{
    struct ofpbuf *msg;

    struct ofp13_meter_mod *omm;

    msg = ofpraw_alloc(OFPRAW_OFPT13_METER_MOD, ofp_version,
                       NXM_TYPICAL_LEN + mm->meter.n_bands * 16);
    omm = ofpbuf_put_zeros(msg, sizeof *omm);
    omm->command = htons(mm->command);
    if (mm->command != OFPMC13_DELETE) {
        omm->flags = htons(mm->meter.flags);
    }
    omm->meter_id = htonl(mm->meter.meter_id);

    ofputil_put_bands(mm->meter.n_bands, mm->meter.bands, msg);

    ofpmsg_update_length(msg);
    return msg;
}
