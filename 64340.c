ofproto_port_dump_start(struct ofproto_port_dump *dump,
                        const struct ofproto *ofproto)
{
    dump->ofproto = ofproto;
    dump->error = ofproto->ofproto_class->port_dump_start(ofproto,
                                                          &dump->state);
}
