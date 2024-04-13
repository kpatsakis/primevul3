parse_RESUBMIT(char *arg, struct ofpbuf *ofpacts,
               enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    struct ofpact_resubmit *resubmit;
    char *in_port_s, *table_s;

    resubmit = ofpact_put_RESUBMIT(ofpacts);

    in_port_s = strsep(&arg, ",");
    if (in_port_s && in_port_s[0]) {
        if (!ofputil_port_from_string(in_port_s, &resubmit->in_port)) {
            return xasprintf("%s: resubmit to unknown port", in_port_s);
        }
    } else {
        resubmit->in_port = OFPP_IN_PORT;
    }

    table_s = strsep(&arg, ",");
    if (table_s && table_s[0]) {
        uint32_t table_id = 0;
        char *error;

        error = str_to_u32(table_s, &table_id);
        if (error) {
            return error;
        }
        resubmit->table_id = table_id;
    } else {
        resubmit->table_id = 255;
    }

    if (resubmit->in_port == OFPP_IN_PORT && resubmit->table_id == 255) {
        return xstrdup("at least one \"in_port\" or \"table\" must be "
                       "specified  on resubmit");
    }
    return NULL;
}
