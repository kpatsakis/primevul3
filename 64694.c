parse_table_mod_vacancy_property(struct ofpbuf *property,
                                 struct ofputil_table_mod *tm)
{
    struct ofp14_table_mod_prop_vacancy *otv = property->data;

    if (property->size != sizeof *otv) {
        return OFPERR_OFPBPC_BAD_LEN;
    }
    tm->table_vacancy.vacancy_down = otv->vacancy_down;
    tm->table_vacancy.vacancy_up = otv->vacancy_up;
    if (tm->table_vacancy.vacancy_down > tm->table_vacancy.vacancy_up) {
        OFPPROP_LOG(&bad_ofmsg_rl, false,
                    "Value of vacancy_down is greater than vacancy_up");
        return OFPERR_OFPBPC_BAD_VALUE;
    }
    if (tm->table_vacancy.vacancy_down > 100 ||
        tm->table_vacancy.vacancy_up > 100) {
        OFPPROP_LOG(&bad_ofmsg_rl, false, "Vacancy threshold percentage "
                    "should not be greater than 100");
        return OFPERR_OFPBPC_BAD_VALUE;
    }
    tm->table_vacancy.vacancy = otv->vacancy;
    if (tm->table_vacancy.vacancy) {
        OFPPROP_LOG(&bad_ofmsg_rl, false,
                    "Vacancy value should be zero for table-mod messages");
        return OFPERR_OFPBPC_BAD_VALUE;
    }
    return 0;
}
