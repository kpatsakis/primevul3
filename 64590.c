ofputil_group_to_string(uint32_t group_id,
                        char namebuf[MAX_GROUP_NAME_LEN + 1], size_t bufsize)
{
    switch (group_id) {
    case OFPG_ALL:
        ovs_strlcpy(namebuf, "ALL", bufsize);
        break;

    case OFPG_ANY:
        ovs_strlcpy(namebuf, "ANY", bufsize);
        break;

    default:
        snprintf(namebuf, bufsize, "%"PRIu32, group_id);
        break;
    }
}
