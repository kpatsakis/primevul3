parse_pop_vlan(struct ofpbuf *ofpacts)
{
    ofpact_put_STRIP_VLAN(ofpacts)->ofpact.raw = OFPAT_RAW11_POP_VLAN;
    return NULL;
}
