ofputil_decode_table_mod(const struct ofp_header *oh,
                         struct ofputil_table_mod *pm)
{
    memset(pm, 0, sizeof *pm);
    pm->miss = OFPUTIL_TABLE_MISS_DEFAULT;
    pm->eviction = OFPUTIL_TABLE_EVICTION_DEFAULT;
    pm->eviction_flags = UINT32_MAX;
    pm->vacancy = OFPUTIL_TABLE_VACANCY_DEFAULT;

    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPT11_TABLE_MOD) {
        const struct ofp11_table_mod *otm = b.data;

        pm->table_id = otm->table_id;
        pm->miss = ofputil_decode_table_miss(otm->config, oh->version);
    } else if (raw == OFPRAW_OFPT14_TABLE_MOD) {
        const struct ofp14_table_mod *otm = ofpbuf_pull(&b, sizeof *otm);

        pm->table_id = otm->table_id;
        pm->miss = ofputil_decode_table_miss(otm->config, oh->version);
        pm->eviction = ofputil_decode_table_eviction(otm->config, oh->version);
        pm->vacancy = ofputil_decode_table_vacancy(otm->config, oh->version);
        while (b.size > 0) {
            struct ofpbuf property;
            enum ofperr error;
            uint64_t type;

            error = ofpprop_pull(&b, &property, &type);
            if (error) {
                return error;
            }

            switch (type) {
            case OFPTMPT14_EVICTION:
                error = ofpprop_parse_u32(&property, &pm->eviction);
                break;

            case OFPTMPT14_VACANCY:
                error = parse_table_mod_vacancy_property(&property, pm);
                break;

            default:
                error = OFPERR_OFPBRC_BAD_TYPE;
                break;
            }

            if (error) {
                return error;
            }
        }
    } else {
        return OFPERR_OFPBRC_BAD_TYPE;
    }

    return 0;
}
