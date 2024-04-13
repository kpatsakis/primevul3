ofputil_format_version_bitmap__(struct ds *msg, uint32_t bitmap,
                                void (*format_version)(struct ds *msg,
                                                       enum ofp_version))
{
    while (bitmap) {
        format_version(msg, raw_ctz(bitmap));
        bitmap = zero_rightmost_1bit(bitmap);
        if (bitmap) {
            ds_put_cstr(msg, ", ");
        }
    }
}
