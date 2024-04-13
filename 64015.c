ofpact_hdrs_hash(const struct ofpact_hdrs *hdrs)
{
    return hash_2words(hdrs->vendor, (hdrs->type << 16) | hdrs->ofp_version);
}
