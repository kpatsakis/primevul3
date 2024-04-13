    OVS_EXCLUDED(ofproto_mutex)
{
    rule_criteria_init(&ofm->criteria, fm->table_id, &fm->match, fm->priority,
                       OVS_VERSION_MAX, fm->cookie, fm->cookie_mask,
                       fm->out_port, fm->out_group);
    rule_criteria_require_rw(&ofm->criteria,
                             (fm->flags & OFPUTIL_FF_NO_READONLY) != 0);
    return 0;
}
