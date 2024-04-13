create_lookup_state(apr_pool_t *result_pool)
{
  lookup_state_t *state = apr_pcalloc(result_pool, sizeof(*state));

  state->next = apr_array_make(result_pool, 4, sizeof(node_t *));
  state->current = apr_array_make(result_pool, 4, sizeof(node_t *));

  /* Virtually all path segments should fit into this buffer.  If they
   * don't, the buffer gets automatically reallocated.
   *
   * Using a smaller initial size would be fine as well but does not
   * buy us much for the increased risk of being expanded anyway - at
   * some extra cost. */
  state->scratch_pad = svn_stringbuf_create_ensure(200, result_pool);

  /* Most paths should fit into this buffer.  The same rationale as
   * above applies. */
  state->parent_path = svn_stringbuf_create_ensure(200, result_pool);

  return state;
}