link_prefix_patterns(apr_array_header_t *array)
{
  int i;
  if (!array)
    return;

  for (i = 1; i < array->nelts; ++i)
    {
      sorted_pattern_t *prev
        = &APR_ARRAY_IDX(array, i - 1, sorted_pattern_t);
      sorted_pattern_t *pattern
        = &APR_ARRAY_IDX(array, i, sorted_pattern_t);

      /* Does PATTERN potentially have a prefix in ARRAY?
       * If so, at least the first char must match with the predecessor's
       * because the array is sorted by that string. */
      if (prev->node->segment.data[0] != pattern->node->segment.data[0])
        continue;

      /* Only the predecessor or any of its prefixes can be the closest
       * prefix to PATTERN. */
      for ( ; prev; prev = prev->next)
        if (   prev->node->segment.len < pattern->node->segment.len
            && !memcmp(prev->node->segment.data,
                       pattern->node->segment.data,
                       prev->node->segment.len))
          {
            pattern->next = prev;
            break;
          }
    }
}