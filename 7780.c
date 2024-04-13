lookup(lookup_state_t *state,
       const char *path,
       authz_access_t required,
       svn_boolean_t recursive,
       apr_pool_t *scratch_pool)
{
  /* Create a scratch pad large enough to hold any of PATH's segments. */
  apr_size_t path_len = strlen(path);
  svn_stringbuf_ensure(state->scratch_pad, path_len);

  /* Normalize start and end of PATH.  Most paths will be fully normalized,
   * so keep the overhead as low as possible. */
  if (path_len && path[path_len-1] == '/')
    {
      do
      {
        --path_len;
      }
      while (path_len && path[path_len-1] == '/');
      path = apr_pstrmemdup(scratch_pool, path, path_len);
    }

  while (path[0] == '/')
    ++path;     /* Don't update PATH_LEN as we won't need it anymore. */

  /* Actually walk the path rule tree following PATH until we run out of
   * either tree or PATH. */
  while (state->current->nelts && path)
    {
      apr_array_header_t *temp;
      int i;
      svn_stringbuf_t *segment = state->scratch_pad;

      /* Shortcut 1: We could nowhere find enough rights in this sub-tree. */
      if ((state->rights.max_rights & required) != required)
        return FALSE;

      /* Shortcut 2: We will find enough rights everywhere in this sub-tree. */
      if ((state->rights.min_rights & required) == required)
        return TRUE;

      /* Extract the next segment. */
      path = next_segment(segment, path);

      /* Initial state for this segment. */
      apr_array_clear(state->next);
      state->rights.access.sequence_number = NO_SEQUENCE_NUMBER;
      state->rights.access.rights = authz_access_none;

      /* These init values ensure that the first node's value will be used
       * when combined with them.  If there is no first node,
       * state->access.sequence_number remains unchanged and we will use
       * the parent's (i.e. inherited) access rights. */
      state->rights.min_rights = authz_access_write;
      state->rights.max_rights = authz_access_none;

      /* Update the PARENT_PATH member in STATE to match the nodes in
       * CURRENT at the end of this iteration, i.e. if and when NEXT
       * has become CURRENT. */
      if (path)
        {
          svn_stringbuf_appendbyte(state->parent_path, '/');
          svn_stringbuf_appendbytes(state->parent_path, segment->data,
                                    segment->len);
        }

      /* Scan follow all alternative routes to the next level. */
      for (i = 0; i < state->current->nelts; ++i)
        {
          node_t *node = APR_ARRAY_IDX(state->current, i, node_t *);
          if (node->sub_nodes)
            add_next_node(state, apr_hash_get(node->sub_nodes, segment->data,
                                              segment->len));

          /* Process alternative, wildcard-based sub-nodes. */
          if (node->pattern_sub_nodes)
            {
              add_next_node(state, node->pattern_sub_nodes->any);

              /* If the current node represents a "**" pattern, it matches
               * to all levels. So, add it to the list for the NEXT level. */
              if (node->pattern_sub_nodes->repeat)
                add_next_node(state, node);

              /* Find all prefix pattern matches. */
              if (node->pattern_sub_nodes->prefixes)
                add_prefix_matches(state, segment,
                                   node->pattern_sub_nodes->prefixes);

              if (node->pattern_sub_nodes->complex)
                add_complex_matches(state, segment,
                                    node->pattern_sub_nodes->complex);

              /* Find all suffux pattern matches.
               * This must be the last check as it destroys SEGMENT. */
              if (node->pattern_sub_nodes->suffixes)
                {
                  /* Suffixes behave like reversed prefixes. */
                  svn_authz__reverse_string(segment->data, segment->len);
                  add_prefix_matches(state, segment,
                                     node->pattern_sub_nodes->suffixes);
                }
            }
        }

      /* If no rule applied to this SEGMENT directly, the parent rights
       * will apply to at least the SEGMENT node itself and possibly
       * other parts deeper in it's subtree. */
      if (!has_local_rule(&state->rights))
        {
          state->rights.access = state->parent_rights.access;
          state->rights.min_rights &= state->parent_rights.access.rights;
          state->rights.max_rights |= state->parent_rights.access.rights;
        }

      /* The list of nodes for SEGMENT is now complete.  If we need to
       * continue, make it the current and put the old one into the recycler.
       *
       * If this is the end of the path, keep the parent path and rights in
       * STATE as are such that sibling lookups will benefit from it.
       */
      if (path)
        {
          temp = state->current;
          state->current = state->next;
          state->next = temp;

          /* In STATE, PARENT_PATH, PARENT_RIGHTS and CURRENT are now in sync. */
          state->parent_rights = state->rights;
        }
    }

  /* If we check recursively, none of the (potential) sub-paths must have
   * less than the REQUIRED access rights.  "Potential" because we don't
   * verify that the respective paths actually exist in the repository.
   */
  if (recursive)
    return (state->rights.min_rights & required) == required;

  /* Return whether the access rights on PATH fully include REQUIRED. */
  return (state->rights.access.rights & required) == required;
}