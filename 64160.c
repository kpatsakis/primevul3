    OVS_REQUIRES(ofproto_mutex)
{
    learned_cookies_update__(ofproto, actions, -1, dead_cookies);
}
