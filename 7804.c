process_acl(construction_context_t *ctx,
            const authz_acl_t *acl,
            node_t *root,
            const char *repository,
            const char *user,
            apr_pool_t *result_pool,
            apr_pool_t *scratch_pool)
{
  path_access_t path_access;
  int i;
  node_t *node;

  /* Skip ACLs that don't say anything about the current user
     and/or repository. */
  if (!svn_authz__get_acl_access(&path_access.rights, acl, user, repository))
    return;

  /* Insert the rule into the filtered tree. */
  path_access.sequence_number = acl->sequence_number;

  /* Try to reuse results from previous runs.
   * Basically, skip the common prefix. */
  node = root;
  for (i = 0; i < ctx->path->nelts; ++i)
    {
      const node_segment_pair_t *step
        = &APR_ARRAY_IDX(ctx->path, i, const node_segment_pair_t);

      /* Exploit the fact that all strings in the authz model are unique /
       * internized and can be identified by address alone. */
      if (   !step->node
          || i >= acl->rule.len
          || step->segment->kind != acl->rule.path[i].kind
          || step->segment->pattern.data != acl->rule.path[i].pattern.data)
        {
          ctx->path->nelts = i;
          break;
        }
      else
        {
          node = step->node;
        }
    }

  /* Insert the path rule into the filtered tree. */
  insert_path(ctx, node, &path_access,
              acl->rule.len - i, acl->rule.path + i,
              result_pool, scratch_pool);
}