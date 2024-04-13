node_is_possible_guard(const node_t *node)
{
  /* The "GUARDS" set is all nodes in the nodelist for which this predicate
   * holds. */

  tor_assert(node);
  return (node->is_possible_guard &&
          node->is_stable &&
          node->is_fast &&
          node->is_valid &&
          node_is_dir(node));
}
