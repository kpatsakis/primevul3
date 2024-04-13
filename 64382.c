ofproto_table_get_miss_config(const struct ofproto *ofproto, uint8_t table_id)
{
    enum ofputil_table_miss miss;

    atomic_read_relaxed(&ofproto->tables[table_id].miss_config, &miss);
    return miss;
}
