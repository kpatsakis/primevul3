router_set_status(const char *digest, int up)
{
  node_t *node;
  tor_assert(digest);

  SMARTLIST_FOREACH(router_get_fallback_dir_servers(),
                    dir_server_t *, d,
                    if (tor_memeq(d->digest, digest, DIGEST_LEN))
                      d->is_running = up);

  SMARTLIST_FOREACH(router_get_trusted_dir_servers(),
                    dir_server_t *, d,
                    if (tor_memeq(d->digest, digest, DIGEST_LEN))
                      d->is_running = up);

  node = node_get_mutable_by_id(digest);
  if (node) {
#if 0
    log_debug(LD_DIR,"Marking router %s as %s.",
              node_describe(node), up ? "up" : "down");
#endif
    if (!up && node_is_me(node) && !net_is_disabled())
      log_warn(LD_NET, "We just marked ourself as down. Are your external "
               "addresses reachable?");

    if (bool_neq(node->is_running, up))
      router_dir_info_changed();

    node->is_running = up;
  }
}
