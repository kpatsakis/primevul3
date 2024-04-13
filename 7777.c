add_complex_matches(lookup_state_t *state,
                    const svn_stringbuf_t *segment,
                    apr_array_header_t *patterns)
{
  int i;
  for (i = 0; i < patterns->nelts; ++i)
    {
      node_t *node = APR_ARRAY_IDX(patterns, i, sorted_pattern_t).node;
      if (0 == apr_fnmatch(node->segment.data, segment->data, 0))
        add_next_node(state, node);
    }
}