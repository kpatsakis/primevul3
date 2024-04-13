finalize_subnode_hash(apr_hash_t *hash,
                      limited_rights_t *sum,
                      apr_pool_t *scratch_pool)
{
  if (hash)
    {
      apr_hash_index_t *hi;
      for (hi = apr_hash_first(scratch_pool, hash);
           hi;
           hi = apr_hash_next(hi))
        finalize_tree(apr_hash_this_val(hi), sum, scratch_pool);
    }
}