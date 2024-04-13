    OVS_REQUIRES(ofproto_mutex)
{
    uint32_t hash = hash_learned_cookie(learn->cookie, learn->table_id);
    struct learned_cookie *c;

    HMAP_FOR_EACH_WITH_HASH (c, u.hmap_node, hash, &ofproto->learned_cookies) {
        if (c->cookie == learn->cookie && c->table_id == learn->table_id) {
            c->n += delta;
            ovs_assert(c->n >= 0);

            if (!c->n) {
                hmap_remove(&ofproto->learned_cookies, &c->u.hmap_node);
                ovs_list_push_back(dead_cookies, &c->u.list_node);
            }

            return;
        }
    }

    ovs_assert(delta > 0);
    c = xmalloc(sizeof *c);
    hmap_insert(&ofproto->learned_cookies, &c->u.hmap_node, hash);
    c->cookie = learn->cookie;
    c->table_id = learn->table_id;
    c->n = delta;
}
