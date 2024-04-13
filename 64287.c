ofport_remove_usage(struct ofproto *ofproto, ofp_port_t ofp_port)
{
    struct ofport_usage *usage;
    HMAP_FOR_EACH_IN_BUCKET (usage, hmap_node, hash_ofp_port(ofp_port),
                             &ofproto->ofport_usage) {
        if (usage->ofp_port == ofp_port) {
            hmap_remove(&ofproto->ofport_usage, &usage->hmap_node);
            free(usage);
            break;
        }
    }
}
