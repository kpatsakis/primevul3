    OVS_REQUIRES(ofproto_mutex)
{
    enum nx_flow_monitor_flags update;

    if (rule_is_hidden(rule)) {
        return;
    }

    if (!ofproto_rule_has_out_port(rule, m->out_port)) {
        return;
    }

    if (seqno) {
        if (rule->add_seqno > seqno) {
            update = NXFMF_ADD | NXFMF_MODIFY;
        } else if (rule->modify_seqno > seqno) {
            update = NXFMF_MODIFY;
        } else {
            return;
        }

        if (!(m->flags & update)) {
            return;
        }
    } else {
        update = NXFMF_INITIAL;
    }

    if (!rule->monitor_flags) {
        rule_collection_add(rules, rule);
    }
    rule->monitor_flags |= update | (m->flags & NXFMF_ACTIONS);
}
