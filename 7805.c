finalize_subnode_array(apr_array_header_t *array,
                       limited_rights_t *sum,
                       apr_pool_t *scratch_pool)
{
  if (array)
    {
      int i;
      for (i = 0; i < array->nelts; ++i)
        finalize_tree(APR_ARRAY_IDX(array, i, sorted_pattern_t).node, sum,
                      scratch_pool);
    }
}