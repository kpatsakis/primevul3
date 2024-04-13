node_is_unreliable(const node_t *node, int need_uptime,
                   int need_capacity, int need_guard)
{
  if (need_uptime && !node->is_stable)
    return 1;
  if (need_capacity && !node->is_fast)
    return 1;
  if (need_guard && !node->is_possible_guard)
    return 1;
  return 0;
}
