insert_path(construction_context_t *ctx,
            node_t *node,
            path_access_t *path_access,
            int segment_count,
            authz_rule_segment_t *segment,
            apr_pool_t *result_pool,
            apr_pool_t *scratch_pool)
{
  node_t *sub_node;
  node_segment_pair_t *node_segment;

  /* End of path? */
  if (segment_count == 0)
    {
      /* Set access rights.  Note that there might be multiple rules for
       * the same path due to non-repo-specific rules vs. repo-specific
       * ones.  Whichever gets defined last wins.
       */
      limited_rights_t rights;
      rights.access = *path_access;
      rights.max_rights = path_access->rights;
      rights.min_rights = path_access->rights;
      combine_access(&node->rights, &rights);
      return;
    }

  /* Any wildcards?  They will go into a separate sub-structure. */
  if (segment->kind != authz_rule_literal)
    ensure_pattern_sub_nodes(node, result_pool);

  switch (segment->kind)
    {
      /* A full wildcard segment? */
    case authz_rule_any_segment:
      sub_node = ensure_node(&node->pattern_sub_nodes->any,
                             segment, result_pool);
      break;

      /* One or more full wildcard segments? */
    case authz_rule_any_recursive:
      sub_node = ensure_node(&node->pattern_sub_nodes->any_var,
                             segment, result_pool);
      ensure_pattern_sub_nodes(sub_node, result_pool)->repeat = TRUE;
      break;

      /* A single wildcard at the end of the segment? */
    case authz_rule_prefix:
      sub_node = ensure_node_in_array(&node->pattern_sub_nodes->prefixes,
                                      segment, result_pool);
      break;

      /* A single wildcard at the start of segments? */
    case authz_rule_suffix:
      sub_node = ensure_node_in_array(&node->pattern_sub_nodes->suffixes,
                                      segment, result_pool);
      break;

      /* General pattern? */
    case authz_rule_fnmatch:
      sub_node = ensure_node_in_array(&node->pattern_sub_nodes->complex,
                                      segment, result_pool);
      break;

      /* Then it must be a literal. */
    default:
      SVN_ERR_ASSERT_NO_RETURN(segment->kind == authz_rule_literal);

      if (!node->sub_nodes)
        {
          node->sub_nodes = svn_hash__make(result_pool);
          sub_node = NULL;
        }
      else
        {
          sub_node = svn_hash_gets(node->sub_nodes, segment->pattern.data);
        }

      /* Auto-insert a sub-node for the current segment. */
      if (!sub_node)
        {
          sub_node = create_node(segment, result_pool);
          apr_hash_set(node->sub_nodes,
                       sub_node->segment.data,
                       sub_node->segment.len,
                       sub_node);
        }
    }

  /* Update context. */
  node_segment = apr_array_push(ctx->path);
  node_segment->segment = segment;
  node_segment->node = sub_node;

  /* Continue at the sub-node with the next segment. */
  insert_path(ctx, sub_node, path_access, segment_count - 1, segment + 1,
              result_pool, scratch_pool);
}