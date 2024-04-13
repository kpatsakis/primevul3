    OVS_REQUIRES(ofproto_mutex)
{
    enum ofperr error;

    if (!ofproto_group_exists(ofproto, ogm->gm.group_id)) {
        error = add_group_start(ofproto, ogm);
    } else {
        error = modify_group_start(ofproto, ogm);
    }

    return error;
}
