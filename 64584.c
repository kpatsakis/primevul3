ofputil_frag_handling_to_string(enum ofputil_frag_handling frag)
{
    switch (frag) {
    case OFPUTIL_FRAG_NORMAL:   return "normal";
    case OFPUTIL_FRAG_DROP:     return "drop";
    case OFPUTIL_FRAG_REASM:    return "reassemble";
    case OFPUTIL_FRAG_NX_MATCH: return "nx-match";
    }

    OVS_NOT_REACHED();
}
