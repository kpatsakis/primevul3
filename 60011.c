pathbias_check_close_success_count(entry_guard_t *node)
{
  const or_options_t *options = get_options();
  const double EPSILON = 1.0e-9;

  /* Note: We rely on the < comparison here to allow us to set a 0
   * rate and disable the feature entirely. If refactoring, don't
   * change to <= */
  if (node->pb.circ_attempts > EPSILON &&
      pathbias_get_close_success_count(node)/node->pb.circ_attempts
      < pathbias_get_extreme_rate(options) &&
      pathbias_get_dropguards(options)) {
    node->pb.path_bias_disabled = 1;
    log_info(LD_GENERAL,
             "Path bias is too high (%f/%f); disabling node %s",
             node->pb.circ_successes, node->pb.circ_attempts,
             node->nickname);
  }
}
