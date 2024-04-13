ofputil_decode_meter_mod(const struct ofp_header *oh,
                         struct ofputil_meter_mod *mm,
                         struct ofpbuf *bands)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&b);
    const struct ofp13_meter_mod *omm = ofpbuf_pull(&b, sizeof *omm);

    /* Translate the message. */
    mm->command = ntohs(omm->command);
    if (mm->command != OFPMC13_ADD &&
        mm->command != OFPMC13_MODIFY &&
        mm->command != OFPMC13_DELETE) {
        return OFPERR_OFPMMFC_BAD_COMMAND;
    }
    mm->meter.meter_id = ntohl(omm->meter_id);

    if (mm->command == OFPMC13_DELETE) {
        mm->meter.flags = 0;
        mm->meter.n_bands = 0;
        mm->meter.bands = NULL;
    } else {
        enum ofperr error;

        mm->meter.flags = ntohs(omm->flags);
        if (mm->meter.flags & OFPMF13_KBPS &&
            mm->meter.flags & OFPMF13_PKTPS) {
            return OFPERR_OFPMMFC_BAD_FLAGS;
        }

        error = ofputil_pull_bands(&b, b.size, &mm->meter.n_bands, bands);
        if (error) {
            return error;
        }
        mm->meter.bands = bands->data;
    }
    return 0;
}
