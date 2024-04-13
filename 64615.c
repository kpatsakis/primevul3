ofputil_port_from_string(const char *s, ofp_port_t *portp)
{
    unsigned int port32; /* int is at least 32 bits wide. */

    if (*s == '-') {
        VLOG_WARN("Negative value %s is not a valid port number.", s);
        return false;
    }
    *portp = 0;
    if (str_to_uint(s, 10, &port32)) {
        if (port32 < ofp_to_u16(OFPP_MAX)) {
            /* Pass. */
        } else if (port32 < ofp_to_u16(OFPP_FIRST_RESV)) {
            VLOG_WARN("port %u is a reserved OF1.0 port number that will "
                      "be translated to %u when talking to an OF1.1 or "
                      "later controller", port32, port32 + OFPP11_OFFSET);
        } else if (port32 <= ofp_to_u16(OFPP_LAST_RESV)) {
            char name[OFP_MAX_PORT_NAME_LEN];

            ofputil_port_to_string(u16_to_ofp(port32), name, sizeof name);
            VLOG_WARN_ONCE("referring to port %s as %"PRIu32" is deprecated "
                           "for compatibility with OpenFlow 1.1 and later",
                           name, port32);
        } else if (port32 < ofp11_to_u32(OFPP11_MAX)) {
            VLOG_WARN("port %u is outside the supported range 0 through "
                      "%"PRIx16" or 0x%x through 0x%"PRIx32, port32,
                      UINT16_MAX, ofp11_to_u32(OFPP11_MAX), UINT32_MAX);
            return false;
        } else {
            port32 -= OFPP11_OFFSET;
        }

        *portp = u16_to_ofp(port32);
        return true;
    } else {
        struct pair {
            const char *name;
            ofp_port_t value;
        };
        static const struct pair pairs[] = {
#define OFPUTIL_NAMED_PORT(NAME) {#NAME, OFPP_##NAME},
            OFPUTIL_NAMED_PORTS_WITH_NONE
#undef OFPUTIL_NAMED_PORT
        };
        const struct pair *p;

        for (p = pairs; p < &pairs[ARRAY_SIZE(pairs)]; p++) {
            if (!strcasecmp(s, p->name)) {
                *portp = p->value;
                return true;
            }
        }
        return false;
    }
}
