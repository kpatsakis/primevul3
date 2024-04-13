get_frac_paths_needed_for_circs(const or_options_t *options,
                                const networkstatus_t *ns)
{
#define DFLT_PCT_USABLE_NEEDED 60
  if (options->PathsNeededToBuildCircuits >= 0.0) {
    return options->PathsNeededToBuildCircuits;
  } else {
    return networkstatus_get_param(ns, "min_paths_for_circs_pct",
                                   DFLT_PCT_USABLE_NEEDED,
                                   25, 95)/100.0;
  }
}
