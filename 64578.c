ofputil_format_version(struct ds *msg, enum ofp_version version)
{
    ds_put_format(msg, "0x%02x", version);
}
