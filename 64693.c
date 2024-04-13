parse_table_desc_vacancy_property(struct ofpbuf *property,
                                  struct ofputil_table_desc *td)
{
    struct ofp14_table_mod_prop_vacancy *otv = property->data;

    if (property->size != sizeof *otv) {
        return OFPERR_OFPBPC_BAD_LEN;
    }

    td->table_vacancy.vacancy_down = otv->vacancy_down;
    td->table_vacancy.vacancy_up = otv->vacancy_up;
    td->table_vacancy.vacancy = otv->vacancy;
    return 0;
}
