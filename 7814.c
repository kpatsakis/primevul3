add_prefix_matches(lookup_state_t *state,
                   const svn_stringbuf_t *segment,
                   apr_array_header_t *prefixes)
{
  /* Index of the first node that might be a match.  All matches will
   * be at this and the immediately following indexes. */
  int i = svn_sort__bsearch_lower_bound(prefixes, segment->data,
                                        compare_node_path_segment);

  /* The entry we found may be an exact match (but not a true prefix).
   * The prefix matching test will still work. */
  if (i < prefixes->nelts)
    add_if_prefix_matches(state,
                          &APR_ARRAY_IDX(prefixes, i, sorted_pattern_t),
                          segment);

  /* The immediate predecessor may be a true prefix and all potential
   * prefixes can be found following the NEXT links between the array
   * indexes. */
  if (i > 0)
    {
      sorted_pattern_t *pattern;
      for (pattern = &APR_ARRAY_IDX(prefixes, i - 1, sorted_pattern_t);
           pattern;
           pattern = pattern->next)
        {
          add_if_prefix_matches(state, pattern, segment);
        }
    }
}