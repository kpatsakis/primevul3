    OVS_REQUIRES(ofproto_mutex)
{
    struct ofgroup *old_group;          /* Modified group. */
    struct ofgroup *new_group;
    enum ofperr error;

    old_group = ofproto_group_lookup__(ofproto, ogm->gm.group_id,
                                       OVS_VERSION_MAX);
    if (!old_group) {
        return OFPERR_OFPGMFC_UNKNOWN_GROUP;
    }

    /* Inserting or deleting a bucket should not change the group's type or
     * properties, so change the group mod so that these aspects match the old
     * group.  (See EXT-570.)  */
    if (ogm->gm.command == OFPGC15_INSERT_BUCKET ||
        ogm->gm.command == OFPGC15_REMOVE_BUCKET) {
        ogm->gm.type = old_group->type;
        ofputil_group_properties_destroy(&ogm->gm.props);
        ofputil_group_properties_copy(&ogm->gm.props, &old_group->props);
    }

    if (old_group->type != ogm->gm.type
        && (ofproto->n_groups[ogm->gm.type]
            >= ofproto->ogf.max_groups[ogm->gm.type])) {
        return OFPERR_OFPGMFC_OUT_OF_GROUPS;
    }

    error = init_group(ofproto, &ogm->gm, ogm->version, &ogm->new_group);
    if (error) {
        return error;
    }
    new_group = ogm->new_group;

    /* Manipulate bucket list for bucket commands */
    if (ogm->gm.command == OFPGC15_INSERT_BUCKET) {
        error = copy_buckets_for_insert_bucket(old_group, new_group,
                                               ogm->gm.command_bucket_id);
    } else if (ogm->gm.command == OFPGC15_REMOVE_BUCKET) {
        error = copy_buckets_for_remove_bucket(old_group, new_group,
                                               ogm->gm.command_bucket_id);
    }
    if (error) {
        goto out;
    }

    /* The group creation time does not change during modification. */
    *CONST_CAST(long long int *, &(new_group->created)) = old_group->created;
    *CONST_CAST(long long int *, &(new_group->modified)) = time_msec();

    group_collection_add(&ogm->old_groups, old_group);

    /* Mark the old group for deletion. */
    versions_set_remove_version(&old_group->versions, ogm->version);
    /* Insert replacement group. */
    cmap_insert(&ofproto->groups, &new_group->cmap_node,
                    hash_int(new_group->group_id, 0));
    /* Transfer rules. */
    rule_collection_move(&new_group->rules, &old_group->rules);

    if (old_group->type != new_group->type) {
        ofproto->n_groups[old_group->type]--;
        ofproto->n_groups[new_group->type]++;
    }
    return 0;

out:
    ofproto_group_unref(new_group);
    return error;
}
