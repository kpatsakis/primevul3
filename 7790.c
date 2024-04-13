add_if_prefix_matches(lookup_state_t *state,
                      const sorted_pattern_t *prefix,
                      const svn_stringbuf_t *segment)
{
  node_t *node = prefix->node;
  if (   node->segment.len <= segment->len
      && !memcmp(node->segment.data, segment->data, node->segment.len))
    add_next_node(state, node);
}