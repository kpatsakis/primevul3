    OVS_REQUIRES(ofproto_mutex)
{
    struct ofgroup *new_group = ogm->new_group;
    struct ofgroup *old_group;

    if (new_group && group_collection_n(&ogm->old_groups) &&
        ofproto->ofproto_class->group_modify) {
        /* Modify a group. */
        ovs_assert(group_collection_n(&ogm->old_groups) == 1);

        /* XXX: OK to lose old group's stats? */
        ofproto->ofproto_class->group_modify(new_group);
    }

    /* Delete old groups. */
    GROUP_COLLECTION_FOR_EACH(old_group, &ogm->old_groups) {
        delete_group_finish(ofproto, old_group);
    }
    remove_groups_postponed(&ogm->old_groups);

    if (req) {
        struct ofputil_requestforward rf;
        rf.xid = req->request->xid;
        rf.reason = OFPRFR_GROUP_MOD;
        rf.group_mod = &ogm->gm;
        connmgr_send_requestforward(ofproto->connmgr, req->ofconn, &rf);
    }
}
