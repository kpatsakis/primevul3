count_usable_descriptors(int *num_present, int *num_usable,
                         smartlist_t *descs_out,
                         const networkstatus_t *consensus,
                         const or_options_t *options, time_t now,
                         routerset_t *in_set,
                         usable_descriptor_t exit_only)
{
  const int md = (consensus->flavor == FLAV_MICRODESC);
  *num_present = 0, *num_usable = 0;

  SMARTLIST_FOREACH_BEGIN(consensus->routerstatus_list, routerstatus_t *, rs)
    {
       const node_t *node = node_get_by_id(rs->identity_digest);
       if (!node)
         continue; /* This would be a bug: every entry in the consensus is
                    * supposed to have a node. */
       if (exit_only == USABLE_DESCRIPTOR_EXIT_ONLY && ! rs->is_exit)
         continue;
       if (in_set && ! routerset_contains_routerstatus(in_set, rs, -1))
         continue;
       if (client_would_use_router(rs, now, options)) {
         const char * const digest = rs->descriptor_digest;
         int present;
         ++*num_usable; /* the consensus says we want it. */
         if (md)
           present = NULL != microdesc_cache_lookup_by_digest256(NULL, digest);
         else
           present = NULL != router_get_by_descriptor_digest(digest);
         if (present) {
           /* we have the descriptor listed in the consensus. */
           ++*num_present;
         }
         if (descs_out)
           smartlist_add(descs_out, (node_t*)node);
       }
     }
  SMARTLIST_FOREACH_END(rs);

  log_debug(LD_DIR, "%d usable, %d present (%s%s).",
            *num_usable, *num_present,
            md ? "microdesc" : "desc",
            exit_only == USABLE_DESCRIPTOR_EXIT_ONLY ? " exits" : "s");
}
