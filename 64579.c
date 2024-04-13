ofputil_format_version_bitmap(struct ds *msg, uint32_t bitmap)
{
    ofputil_format_version_bitmap__(msg, bitmap, ofputil_format_version);
}
