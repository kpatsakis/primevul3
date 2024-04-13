ensure_node_in_array(apr_array_header_t **array,
                     authz_rule_segment_t *segment,
                     apr_pool_t *result_pool)
{
  int idx;
  sorted_pattern_t entry;
  sorted_pattern_t *entry_ptr;

  /* Auto-create the array. */
  if (!*array)
    *array = apr_array_make(result_pool, 4, sizeof(sorted_pattern_t));

  /* Find the node in ARRAY and the IDX at which it were to be inserted.
   * Initialize IDX such that we won't attempt a hinted lookup (likely
   * to fail and therefore pure overhead). */
  idx = (*array)->nelts;
  entry_ptr = svn_sort__array_lookup(*array, segment, &idx,
                                     compare_node_rule_segment);
  if (entry_ptr)
    return entry_ptr->node;

  /* There is no such node, yet.
   * Create one and insert it into the sorted array. */
  entry.node = create_node(segment, result_pool);
  entry.next = NULL;
  svn_error_clear(svn_sort__array_insert2(*array, &entry, idx));

  return entry.node;
}