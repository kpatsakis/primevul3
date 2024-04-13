ofputil_format_port(ofp_port_t port, struct ds *s)
{
    char name[OFP_MAX_PORT_NAME_LEN];

    ofputil_port_to_string(port, name, sizeof name);
    ds_put_cstr(s, name);
}
