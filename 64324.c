ofproto_init(const struct shash *iface_hints)
{
    struct shash_node *node;
    size_t i;

    ofproto_class_register(&ofproto_dpif_class);

    /* Make a local copy, since we don't own 'iface_hints' elements. */
    SHASH_FOR_EACH(node, iface_hints) {
        const struct iface_hint *orig_hint = node->data;
        struct iface_hint *new_hint = xmalloc(sizeof *new_hint);
        const char *br_type = ofproto_normalize_type(orig_hint->br_type);

        new_hint->br_name = xstrdup(orig_hint->br_name);
        new_hint->br_type = xstrdup(br_type);
        new_hint->ofp_port = orig_hint->ofp_port;

        shash_add(&init_ofp_ports, node->name, new_hint);
    }

    for (i = 0; i < n_ofproto_classes; i++) {
        ofproto_classes[i]->init(&init_ofp_ports);
    }

    ofproto_unixctl_init();
}
