create_node(authz_rule_segment_t *segment,
            apr_pool_t *result_pool)
{
  node_t *result = apr_pcalloc(result_pool, sizeof(*result));
  if (segment)
    result->segment = segment->pattern;
  else
    {
      result->segment.data = "";
      result->segment.len = 0;
    }
  result->rights.access.sequence_number = NO_SEQUENCE_NUMBER;
  return result;
}