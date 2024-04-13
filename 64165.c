    OVS_REQUIRES(ofproto_mutex)
{
    hindex_insert(&ofproto->cookies, &rule->cookie_node,
                  hash_cookie(rule->flow_cookie));
}
