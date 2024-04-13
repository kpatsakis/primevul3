ofputil_append_ipfix_stat(struct ovs_list *replies,
                         const struct ofputil_ipfix_stats *ois)
{
    struct nx_ipfix_stats_reply *reply = ofpmp_append(replies, sizeof *reply);
    ofputil_ipfix_stats_to_reply(ois, reply);
}
