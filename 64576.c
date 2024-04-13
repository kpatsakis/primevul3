ofputil_format_group(uint32_t group_id, struct ds *s)
{
    char name[MAX_GROUP_NAME_LEN];

    ofputil_group_to_string(group_id, name, sizeof name);
    ds_put_cstr(s, name);
}
