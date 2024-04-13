ofputil_group_properties_copy(struct ofputil_group_props *to,
                              const struct ofputil_group_props *from)
{
    *to = *from;
    to->fields.values = xmemdup(from->fields.values, from->fields.values_size);
}
