rule_criteria_init(struct rule_criteria *criteria, uint8_t table_id,
                   const struct match *match, int priority,
                   ovs_version_t version, ovs_be64 cookie,
                   ovs_be64 cookie_mask, ofp_port_t out_port,
                   uint32_t out_group)
{
    criteria->table_id = table_id;
    cls_rule_init(&criteria->cr, match, priority);
    criteria->version = version;
    criteria->cookie = cookie;
    criteria->cookie_mask = cookie_mask;
    criteria->out_port = out_port;
    criteria->out_group = out_group;

    /* We ordinarily want to skip hidden rules, but there has to be a way for
     * code internal to OVS to modify and delete them, so if the criteria
     * specify a priority that can only be for a hidden flow, then allow hidden
     * rules to be selected.  (This doesn't allow OpenFlow clients to meddle
     * with hidden flows because OpenFlow uses only a 16-bit field to specify
     * priority.) */
    criteria->include_hidden = priority > UINT16_MAX;

    /* We assume that the criteria are being used to collect flows for reading
     * but not modification.  Thus, we should collect read-only flows. */
    criteria->include_readonly = true;
}
