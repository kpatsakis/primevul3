ofproto_port_dump_next(struct ofproto_port_dump *dump,
                       struct ofproto_port *port)
{
    const struct ofproto *ofproto = dump->ofproto;

    if (dump->error) {
        return false;
    }

    dump->error = ofproto->ofproto_class->port_dump_next(ofproto, dump->state,
                                                         port);
    if (dump->error) {
        ofproto->ofproto_class->port_dump_done(ofproto, dump->state);
        return false;
    }
    return true;
}
