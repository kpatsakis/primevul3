ofputil_tid_command(const struct ofputil_flow_mod *fm,
                    enum ofputil_protocol protocol)
{
    return htons(protocol & OFPUTIL_P_TID
                 ? (fm->command & 0xff) | (fm->table_id << 8)
                 : fm->command);
}
