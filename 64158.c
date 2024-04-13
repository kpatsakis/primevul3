    OVS_REQUIRES(ofproto_mutex)
{
    if (actions->has_learn_with_delete) {
        const struct ofpact_learn *learn;

        for (learn = next_learn_with_delete(actions, NULL); learn;
             learn = next_learn_with_delete(actions, learn)) {
            learned_cookies_update_one__(ofproto, learn, delta, dead_cookies);
        }
    }
}
