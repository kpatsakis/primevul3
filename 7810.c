trim_tree(node_t *node,
          int latest_any_var,
          apr_pool_t *scratch_pool)
{
  svn_boolean_t removed_all = TRUE;

  /* For convenience, we allow NODE to be NULL: */
  if (!node)
    return TRUE;

  /* Do we have a later "any_var" rule at this node. */
  if (   node->pattern_sub_nodes
      && node->pattern_sub_nodes->any_var
      &&   node->pattern_sub_nodes->any_var->rights.access.sequence_number
         > latest_any_var)
    {
      latest_any_var
        = node->pattern_sub_nodes->any_var->rights.access.sequence_number;
    }

  /* Is there a local rule at this node that is not eclipsed by any_var? */
  if (has_local_rule(&node->rights))
    {
      /* Remove the local rule, if it got eclipsed.
       * Note that for the latest any_var node, the sequence number is equal. */
      if (node->rights.access.sequence_number >= latest_any_var)
        removed_all = FALSE;
      else
         node->rights.access.sequence_number = NO_SEQUENCE_NUMBER;
    }

  /* Process all sub-nodes. */
  removed_all &= trim_subnode_hash(&node->sub_nodes, latest_any_var,
                                   scratch_pool);

  if (node->pattern_sub_nodes)
    {
      if (trim_tree(node->pattern_sub_nodes->any, latest_any_var,
                    scratch_pool))
        node->pattern_sub_nodes->any = NULL;
      else
        removed_all = FALSE;

      if (trim_tree(node->pattern_sub_nodes->any_var, latest_any_var,
                    scratch_pool))
        node->pattern_sub_nodes->any_var = NULL;
      else
        removed_all = FALSE;

      removed_all &= trim_subnode_array(&node->pattern_sub_nodes->prefixes,
                                        latest_any_var, scratch_pool);
      removed_all &= trim_subnode_array(&node->pattern_sub_nodes->suffixes,
                                        latest_any_var, scratch_pool);
      removed_all &= trim_subnode_array(&node->pattern_sub_nodes->complex,
                                        latest_any_var, scratch_pool);

      /* Trim the tree as much as possible to speed up lookup(). */
      if (removed_all)
        node->pattern_sub_nodes = NULL;
    }

  return removed_all;
}