    OVS_REQUIRES(ofproto_mutex)
{
    for (uint32_t mid = first; mid <= last; ++mid) {
        struct meter *meter = ofproto->meters[mid];
        if (meter) {
            /* First delete the rules that use this meter. */
            if (!ovs_list_is_empty(&meter->rules)) {
                struct rule_collection rules;
                struct rule *rule;

                rule_collection_init(&rules);

                LIST_FOR_EACH (rule, meter_list_node, &meter->rules) {
                    rule_collection_add(&rules, rule);
                }
                delete_flows__(&rules, OFPRR_METER_DELETE, NULL);
            }

            ofproto->meters[mid] = NULL;
            ofproto->ofproto_class->meter_del(ofproto,
                                              meter->provider_meter_id);
            free(meter->bands);
            free(meter);
        }
    }
}
