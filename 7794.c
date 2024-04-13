trim_subnode_hash(apr_hash_t **hash,
                  int latest_any_var,
                  apr_pool_t *scratch_pool)
{
  if (*hash)
    {
      apr_array_header_t *to_remove = apr_array_make(scratch_pool, 0,
                                                     sizeof(node_t *));

      apr_hash_index_t *hi;
      for (hi = apr_hash_first(scratch_pool, *hash);
           hi;
           hi = apr_hash_next(hi))
        {
          node_t *node = apr_hash_this_val(hi);
          if (trim_tree(node, latest_any_var, scratch_pool))
            APR_ARRAY_PUSH(to_remove, node_t *) = node;
        }

      /* Are some nodes left? */
      if (to_remove->nelts < apr_hash_count(*hash))
        {
          /* Remove empty nodes (if any). */
          int i;
          for (i = 0; i < to_remove->nelts; ++i)
            {
              node_t *node = APR_ARRAY_IDX(to_remove, i, node_t *);
              apr_hash_set(*hash, node->segment.data, node->segment.len,
                           NULL);
            }

          return FALSE;
        }

      /* No nodes left.  A NULL hash is more efficient than an empty one. */
      *hash = NULL;
    }

  return TRUE;
}