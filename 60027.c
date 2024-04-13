count_loading_descriptors_progress(void)
{
  int num_present = 0, num_usable=0;
  time_t now = time(NULL);
  const or_options_t *options = get_options();
  const networkstatus_t *consensus =
    networkstatus_get_reasonably_live_consensus(now,usable_consensus_flavor());
  double paths, fraction;

  if (!consensus)
    return 0; /* can't count descriptors if we have no list of them */

  paths = compute_frac_paths_available(consensus, options, now,
                                       &num_present, &num_usable,
                                       NULL);

  fraction = paths / get_frac_paths_needed_for_circs(options,consensus);
  if (fraction > 1.0)
    return 0; /* it's not the number of descriptors holding us back */
  return BOOTSTRAP_STATUS_LOADING_DESCRIPTORS + (int)
    (fraction*(BOOTSTRAP_STATUS_CONN_OR-1 -
               BOOTSTRAP_STATUS_LOADING_DESCRIPTORS));
}
