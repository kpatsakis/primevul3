parse_DEC_TTL(char *arg, struct ofpbuf *ofpacts,
              enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    if (*arg == '\0') {
        parse_noargs_dec_ttl(ofpacts);
    } else {
        struct ofpact_cnt_ids *ids;
        char *cntr;

        ids = ofpact_put_DEC_TTL(ofpacts);
        ids->ofpact.raw = NXAST_RAW_DEC_TTL_CNT_IDS;
        for (cntr = strtok_r(arg, ", ", &arg); cntr != NULL;
             cntr = strtok_r(NULL, ", ", &arg)) {
            uint16_t id = atoi(cntr);

            ofpbuf_put(ofpacts, &id, sizeof id);
            ids = ofpacts->header;
            ids->n_controllers++;
        }
        if (!ids->n_controllers) {
            return xstrdup("dec_ttl_cnt_ids: expected at least one controller "
                           "id.");
        }
        ofpact_finish_DEC_TTL(ofpacts, &ids);
    }
    return NULL;
}
