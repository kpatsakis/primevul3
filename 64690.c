parse_oxms(struct ofpbuf *payload, bool loose,
           struct mf_bitmap *exactp, struct mf_bitmap *maskedp)
{
    struct mf_bitmap exact = MF_BITMAP_INITIALIZER;
    struct mf_bitmap masked = MF_BITMAP_INITIALIZER;

    while (payload->size > 0) {
        const struct mf_field *field;
        enum ofperr error;
        bool hasmask;

        error = nx_pull_header(payload, NULL, &field, &hasmask);
        if (!error) {
            bitmap_set1(hasmask ? masked.bm : exact.bm, field->id);
        } else if (error != OFPERR_OFPBMC_BAD_FIELD || !loose) {
            return error;
        }
    }
    if (exactp) {
        *exactp = exact;
    } else if (!bitmap_is_all_zeros(exact.bm, MFF_N_IDS)) {
        return OFPERR_OFPBMC_BAD_MASK;
    }
    if (maskedp) {
        *maskedp = masked;
    } else if (!bitmap_is_all_zeros(masked.bm, MFF_N_IDS)) {
        return OFPERR_OFPBMC_BAD_MASK;
    }
    return 0;
}
