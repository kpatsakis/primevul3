ofport_set_usage(struct ofproto *ofproto, ofp_port_t ofp_port,
                 long long int last_used)
{
    struct ofport_usage *usage;
    HMAP_FOR_EACH_IN_BUCKET (usage, hmap_node, hash_ofp_port(ofp_port),
                             &ofproto->ofport_usage) {
        if (usage->ofp_port == ofp_port) {
            usage->last_used = last_used;
            return;
        }
    }
    ovs_assert(last_used == LLONG_MAX);

    usage = xmalloc(sizeof *usage);
    usage->ofp_port = ofp_port;
    usage->last_used = last_used;
    hmap_insert(&ofproto->ofport_usage, &usage->hmap_node,
                hash_ofp_port(ofp_port));
}
