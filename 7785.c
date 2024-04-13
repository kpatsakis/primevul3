compare_node_path_segment(const void *void_lhs,
                          const void *void_rhs)
{
  const sorted_pattern_t *element = void_lhs;
  const char *segment = void_rhs;

  return strcmp(element->node->segment.data, segment);
}