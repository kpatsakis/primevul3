ofputil_group_properties_destroy(struct ofputil_group_props *gp)
{
    free(gp->fields.values);
}
