ofputil_port_to_string(ofp_port_t port,
                       char namebuf[OFP_MAX_PORT_NAME_LEN], size_t bufsize)
{
    switch (port) {
#define OFPUTIL_NAMED_PORT(NAME)                        \
        case OFPP_##NAME:                               \
            ovs_strlcpy(namebuf, #NAME, bufsize);       \
            break;
        OFPUTIL_NAMED_PORTS
#undef OFPUTIL_NAMED_PORT

    default:
        snprintf(namebuf, bufsize, "%"PRIu32, port);
        break;
    }
}
