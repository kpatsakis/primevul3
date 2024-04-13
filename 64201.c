    OVS_REQUIRES(ofproto_mutex)
{
    enum ofperr error;

    ogm->new_group = NULL;
    group_collection_init(&ogm->old_groups);

    switch (ogm->gm.command) {
    case OFPGC11_ADD:
        error = add_group_start(ofproto, ogm);
        break;

    case OFPGC11_MODIFY:
        error = modify_group_start(ofproto, ogm);
        break;

    case OFPGC11_ADD_OR_MOD:
        error = add_or_modify_group_start(ofproto, ogm);
        break;

    case OFPGC11_DELETE:
        delete_groups_start(ofproto, ogm);
        error = 0;
        break;

    case OFPGC15_INSERT_BUCKET:
        error = modify_group_start(ofproto, ogm);
        break;

    case OFPGC15_REMOVE_BUCKET:
        error = modify_group_start(ofproto, ogm);
        break;

    default:
        if (ogm->gm.command > OFPGC11_DELETE) {
            VLOG_INFO_RL(&rl, "%s: Invalid group_mod command type %d",
                         ofproto->name, ogm->gm.command);
        }
        error = OFPERR_OFPGMFC_BAD_COMMAND;
        break;
    }
    return error;
}
