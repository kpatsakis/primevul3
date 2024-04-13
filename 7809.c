compare_node_rule_segment(const void *void_lhs,
                          const void *void_rhs)
{
  const sorted_pattern_t *element = void_lhs;
  const authz_rule_segment_t *segment = void_rhs;

  return strcmp(element->node->segment.data, segment->pattern.data);
}