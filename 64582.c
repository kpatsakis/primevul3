ofputil_format_version_name(struct ds *msg, enum ofp_version version)
{
    ds_put_cstr(msg, ofputil_version_to_string(version));
}
