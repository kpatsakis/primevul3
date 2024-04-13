encode_tlv_table_mappings(struct ofpbuf *b, struct ovs_list *mappings)
{
    struct ofputil_tlv_map *map;

    LIST_FOR_EACH (map, list_node, mappings) {
        struct nx_tlv_map *nx_map;

        nx_map = ofpbuf_put_zeros(b, sizeof *nx_map);
        nx_map->option_class = htons(map->option_class);
        nx_map->option_type = map->option_type;
        nx_map->option_len = map->option_len;
        nx_map->index = htons(map->index);
    }
}
