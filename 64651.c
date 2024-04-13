ofputil_put_group_prop_ntr_selection_method(enum ofp_version ofp_version,
                                            const struct ofputil_group_props *gp,
                                            struct ofpbuf *openflow)
{
    struct ntr_group_prop_selection_method *prop;
    size_t start;

    start = openflow->size;
    ofpbuf_put_zeros(openflow, sizeof *prop);
    oxm_put_field_array(openflow, &gp->fields, ofp_version);
    prop = ofpbuf_at_assert(openflow, start, sizeof *prop);
    prop->type = htons(OFPGPT15_EXPERIMENTER);
    prop->experimenter = htonl(NTR_VENDOR_ID);
    prop->exp_type = htonl(NTRT_SELECTION_METHOD);
    strcpy(prop->selection_method, gp->selection_method);
    prop->selection_method_param = htonll(gp->selection_method_param);
    ofpprop_end(openflow, start);
}
