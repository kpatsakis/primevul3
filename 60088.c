router_exit_policy_all_nodes_reject(const tor_addr_t *addr, uint16_t port,
                                    int need_uptime)
{
  addr_policy_result_t r;

  SMARTLIST_FOREACH_BEGIN(nodelist_get_list(), const node_t *, node) {
    if (node->is_running &&
        !node_is_unreliable(node, need_uptime, 0, 0)) {

      r = compare_tor_addr_to_node_policy(addr, port, node);

      if (r != ADDR_POLICY_REJECTED && r != ADDR_POLICY_PROBABLY_REJECTED)
        return 0; /* this one could be ok. good enough. */
    }
  } SMARTLIST_FOREACH_END(node);
  return 1; /* all will reject. */
}
