send_table_status(struct ofproto *ofproto, uint8_t table_id)
{
    struct oftable *t = &ofproto->tables[table_id];
    if (!t->vacancy_event) {
        return;
    }

    uint8_t vacancy = oftable_vacancy(t);
    enum ofp14_table_reason event;
    if (vacancy < t->vacancy_down) {
        event = OFPTR_VACANCY_DOWN;
    } else if (vacancy > t->vacancy_up) {
        event = OFPTR_VACANCY_UP;
    } else {
        return;
    }

    if (event == t->vacancy_event) {
        struct ofputil_table_desc td;
        query_table_desc__(&td, ofproto, table_id);
        connmgr_send_table_status(ofproto->connmgr, &td, event);

        t->vacancy_event = (event == OFPTR_VACANCY_DOWN
                            ? OFPTR_VACANCY_UP
                            : OFPTR_VACANCY_DOWN);
    }
}
