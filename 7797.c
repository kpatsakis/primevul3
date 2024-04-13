init_lockup_state(lookup_state_t *state,
                  node_t *root,
                  const char *path)
{
  apr_size_t len = strlen(path);
  if (   (len > state->parent_path->len)
      && state->parent_path->len
      && (path[state->parent_path->len] == '/')
      && !memcmp(path, state->parent_path->data, state->parent_path->len))
    {
      /* The PARENT_PATH of the previous lookup is actually a parent path
       * of PATH.  The CURRENT node list already matches the parent path
       * and we only have to set the correct rights info. */
      state->rights = state->parent_rights;

      /* Tell the caller where to proceed. */
      return path + state->parent_path->len;
    }

  /* Start lookup at ROOT for the full PATH. */
  state->rights = root->rights;
  state->parent_rights = root->rights;

  apr_array_clear(state->next);
  apr_array_clear(state->current);
  APR_ARRAY_PUSH(state->current, node_t *) = root;

  /* Var-segment rules match empty segments as well */
  if (root->pattern_sub_nodes && root->pattern_sub_nodes->any_var)
   {
      node_t *node = root->pattern_sub_nodes->any_var;

      /* This is non-recursive due to ACL normalization. */
      combine_access(&state->rights, &node->rights);
      combine_right_limits(&state->rights, &node->rights);
      APR_ARRAY_PUSH(state->current, node_t *) = node;
   }

  svn_stringbuf_setempty(state->parent_path);
  svn_stringbuf_setempty(state->scratch_pad);

  return path;
}