ofproto_unixctl_list(struct unixctl_conn *conn, int argc OVS_UNUSED,
                     const char *argv[] OVS_UNUSED, void *aux OVS_UNUSED)
{
    struct ofproto *ofproto;
    struct ds results;

    ds_init(&results);
    HMAP_FOR_EACH (ofproto, hmap_node, &all_ofprotos) {
        ds_put_format(&results, "%s\n", ofproto->name);
    }
    unixctl_command_reply(conn, ds_cstr(&results));
    ds_destroy(&results);
}
