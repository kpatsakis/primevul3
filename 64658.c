ofputil_put_ofp14_table_desc(const struct ofputil_table_desc *td,
                             struct ofpbuf *b, enum ofp_version version)
{
    struct ofp14_table_desc *otd;
    struct ofp14_table_mod_prop_vacancy *otv;
    size_t start_otd;

    start_otd = b->size;
    ofpbuf_put_zeros(b, sizeof *otd);

    ofpprop_put_u32(b, OFPTMPT14_EVICTION, td->eviction_flags);

    otv = ofpbuf_put_zeros(b, sizeof *otv);
    otv->type = htons(OFPTMPT14_VACANCY);
    otv->length = htons(sizeof *otv);
    otv->vacancy_down = td->table_vacancy.vacancy_down;
    otv->vacancy_up = td->table_vacancy.vacancy_up;
    otv->vacancy = td->table_vacancy.vacancy;

    otd = ofpbuf_at_assert(b, start_otd, sizeof *otd);
    otd->length = htons(b->size - start_otd);
    otd->table_id = td->table_id;
    otd->config = ofputil_encode_table_config(OFPUTIL_TABLE_MISS_DEFAULT,
                                              td->eviction, td->vacancy,
                                              version);
}
