ofputil_format_version_bitmap_names(struct ds *msg, uint32_t bitmap)
{
    ofputil_format_version_bitmap__(msg, bitmap, ofputil_format_version_name);
}
