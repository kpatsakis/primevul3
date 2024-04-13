add_next_node(lookup_state_t *state,
              node_t *node)
{
  /* Allowing NULL nodes simplifies the caller. */
  if (node)
    {
      /* The rule with the highest sequence number is the one that applies.
       * Not all nodes that we are following have rules that apply directly
       * to this path but are mere intermediates that may only have some
       * matching deep sub-node. */
      combine_access(&state->rights, &node->rights);

      /* The rule tree node can be seen as an overlay of all the nodes that
       * we are following.  Any of them _may_ match eventually, so the min/
       * max possible access rights are a combination of all these sub-trees.
       */
      combine_right_limits(&state->rights, &node->rights);

      /* NODE is now enlisted as a (potential) match for the next segment. */
      APR_ARRAY_PUSH(state->next, node_t *) = node;

      /* Variable length sub-segment sequences apply to the same node as
       * they match empty sequences as well. */
      if (node->pattern_sub_nodes && node->pattern_sub_nodes->any_var)
        {
          node = node->pattern_sub_nodes->any_var;

          /* This is non-recursive due to ACL normalization. */
          combine_access(&state->rights, &node->rights);
          combine_right_limits(&state->rights, &node->rights);
          APR_ARRAY_PUSH(state->next, node_t *) = node;
        }
    }
}