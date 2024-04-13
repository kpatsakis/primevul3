handle_port_mod(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofputil_port_mod pm;
    struct ofport *port;
    enum ofperr error;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    error = ofputil_decode_port_mod(oh, &pm, false);
    if (error) {
        return error;
    }

    error = port_mod_start(ofconn, &pm, &port);
    if (!error) {
        port_mod_finish(ofconn, &pm, port);
    }
    return error;
}
