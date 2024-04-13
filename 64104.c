parse_noargs_dec_ttl(struct ofpbuf *ofpacts)
{
    struct ofpact_cnt_ids *ids;
    uint16_t id = 0;

    ofpact_put_DEC_TTL(ofpacts);
    ofpbuf_put(ofpacts, &id, sizeof id);
    ids = ofpacts->header;
    ids->n_controllers++;
    ofpact_finish_DEC_TTL(ofpacts, &ids);
}
