    OVS_EXCLUDED(ofproto_mutex)
{
    rule_criteria_init(&ofm->criteria, fm->table_id, &fm->match, fm->priority,
                       OVS_VERSION_MAX, fm->cookie, fm->cookie_mask, OFPP_ANY,
                       OFPG_ANY);
    rule_criteria_require_rw(&ofm->criteria,
                             (fm->flags & OFPUTIL_FF_NO_READONLY) != 0);
    /* Must create a new flow in advance for the case that no matches are
     * found.  Also used for template for multiple modified flows. */
    add_flow_init(ofproto, ofm, fm);

    return 0;
}
