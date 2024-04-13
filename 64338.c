ofproto_port_dump_done(struct ofproto_port_dump *dump)
{
    const struct ofproto *ofproto = dump->ofproto;
    if (!dump->error) {
        dump->error = ofproto->ofproto_class->port_dump_done(ofproto,
                                                             dump->state);
    }
    return dump->error == EOF ? 0 : dump->error;
}
