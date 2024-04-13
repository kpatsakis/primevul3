table_mod__(struct oftable *oftable,
            const struct ofputil_table_mod *tm)
{
    if (tm->miss == OFPUTIL_TABLE_MISS_DEFAULT) {
        /* This is how an OFPT_TABLE_MOD decodes if it doesn't specify any
         * table-miss configuration (because the protocol used doesn't have
         * such a concept), so there's nothing to do. */
    } else {
        atomic_store_relaxed(&oftable->miss_config, tm->miss);
    }

    unsigned int new_eviction = oftable->eviction;
    if (tm->eviction == OFPUTIL_TABLE_EVICTION_ON) {
        new_eviction |= EVICTION_OPENFLOW;
    } else if (tm->eviction == OFPUTIL_TABLE_EVICTION_OFF) {
        new_eviction &= ~EVICTION_OPENFLOW;
    }

    if (new_eviction != oftable->eviction) {
        ovs_mutex_lock(&ofproto_mutex);
        oftable_configure_eviction(oftable, new_eviction,
                                   oftable->eviction_fields,
                                   oftable->n_eviction_fields);
        ovs_mutex_unlock(&ofproto_mutex);
    }

    if (tm->vacancy != OFPUTIL_TABLE_VACANCY_DEFAULT) {
        ovs_mutex_lock(&ofproto_mutex);
        oftable->vacancy_down = tm->table_vacancy.vacancy_down;
        oftable->vacancy_up = tm->table_vacancy.vacancy_up;
        if (tm->vacancy == OFPUTIL_TABLE_VACANCY_OFF) {
            oftable->vacancy_event = 0;
        } else if (!oftable->vacancy_event) {
            uint8_t vacancy = oftable_vacancy(oftable);
            oftable->vacancy_event = (vacancy < oftable->vacancy_up
                                      ? OFPTR_VACANCY_UP
                                      : OFPTR_VACANCY_DOWN);
        }
        ovs_mutex_unlock(&ofproto_mutex);
    }
}
