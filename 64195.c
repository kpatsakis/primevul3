    OVS_REQUIRES(ofproto_mutex)
{
    enum ofperr error;

    if (ofproto_group_exists(ofproto, ogm->gm.group_id)) {
        return OFPERR_OFPGMFC_GROUP_EXISTS;
    }

    if (ofproto->n_groups[ogm->gm.type]
        >= ofproto->ogf.max_groups[ogm->gm.type]) {
        return OFPERR_OFPGMFC_OUT_OF_GROUPS;
    }

    /* Allocate new group and initialize it. */
    error = init_group(ofproto, &ogm->gm, ogm->version, &ogm->new_group);
    if (!error) {
        /* Insert new group. */
        cmap_insert(&ofproto->groups, &ogm->new_group->cmap_node,
                    hash_int(ogm->new_group->group_id, 0));
        ofproto->n_groups[ogm->new_group->type]++;
    }
    return error;
}
