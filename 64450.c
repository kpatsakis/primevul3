ofputil_append_table_desc_reply(const struct ofputil_table_desc *td,
                                struct ovs_list *replies,
                                enum ofp_version version)
{
    struct ofpbuf *reply = ofpbuf_from_list(ovs_list_back(replies));
    size_t start_otd;
    struct ofp14_table_desc *otd;

    start_otd = reply->size;
    ofpbuf_put_zeros(reply, sizeof *otd);
    if (td->eviction_flags != UINT32_MAX) {
        ofpprop_put_u32(reply, OFPTMPT14_EVICTION, td->eviction_flags);
    }
    if (td->vacancy == OFPUTIL_TABLE_VACANCY_ON) {
        struct ofp14_table_mod_prop_vacancy *otv;

        otv = ofpprop_put_zeros(reply, OFPTMPT14_VACANCY, sizeof *otv);
        otv->vacancy_down = td->table_vacancy.vacancy_down;
        otv->vacancy_up = td->table_vacancy.vacancy_up;
        otv->vacancy = td->table_vacancy.vacancy;
    }

    otd = ofpbuf_at_assert(reply, start_otd, sizeof *otd);
    otd->length = htons(reply->size - start_otd);
    otd->table_id = td->table_id;
    otd->config = ofputil_encode_table_config(OFPUTIL_TABLE_MISS_DEFAULT,
                                              td->eviction, td->vacancy,
                                              version);
    ofpmp_postappend(replies, start_otd);
}
