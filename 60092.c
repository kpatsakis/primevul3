update_router_have_minimum_dir_info(void)
{
  time_t now = time(NULL);
  int res;
  const or_options_t *options = get_options();
  const networkstatus_t *consensus =
    networkstatus_get_reasonably_live_consensus(now,usable_consensus_flavor());
  int using_md;

  if (!consensus) {
    if (!networkstatus_get_latest_consensus())
      strlcpy(dir_info_status, "We have no usable consensus.",
              sizeof(dir_info_status));
    else
      strlcpy(dir_info_status, "We have no recent usable consensus.",
              sizeof(dir_info_status));
    res = 0;
    goto done;
  }

  using_md = consensus->flavor == FLAV_MICRODESC;

  if (! entry_guards_have_enough_dir_info_to_build_circuits()) {
    strlcpy(dir_info_status, "We're missing descriptors for some of our "
            "primary entry guards", sizeof(dir_info_status));
    res = 0;
    goto done;
  }

  /* Check fraction of available paths */
  {
    char *status = NULL;
    int num_present=0, num_usable=0;
    double paths = compute_frac_paths_available(consensus, options, now,
                                                &num_present, &num_usable,
                                                &status);

    if (paths < get_frac_paths_needed_for_circs(options,consensus)) {
      tor_snprintf(dir_info_status, sizeof(dir_info_status),
                   "We need more %sdescriptors: we have %d/%d, and "
                   "can only build %d%% of likely paths. (We have %s.)",
                   using_md?"micro":"", num_present, num_usable,
                   (int)(paths*100), status);
      tor_free(status);
      res = 0;
      control_event_bootstrap(BOOTSTRAP_STATUS_REQUESTING_DESCRIPTORS, 0);
      goto done;
    }

    tor_free(status);
    res = 1;
  }

 done:

  /* If paths have just become available in this update. */
  if (res && !have_min_dir_info) {
    control_event_client_status(LOG_NOTICE, "ENOUGH_DIR_INFO");
    if (control_event_bootstrap(BOOTSTRAP_STATUS_CONN_OR, 0) == 0) {
      log_notice(LD_DIR,
              "We now have enough directory information to build circuits.");
    }
  }

  /* If paths have just become unavailable in this update. */
  if (!res && have_min_dir_info) {
    int quiet = directory_too_idle_to_fetch_descriptors(options, now);
    tor_log(quiet ? LOG_INFO : LOG_NOTICE, LD_DIR,
        "Our directory information is no longer up-to-date "
        "enough to build circuits: %s", dir_info_status);

    /* a) make us log when we next complete a circuit, so we know when Tor
     * is back up and usable, and b) disable some activities that Tor
     * should only do while circuits are working, like reachability tests
     * and fetching bridge descriptors only over circuits. */
    note_that_we_maybe_cant_complete_circuits();
    have_consensus_path = CONSENSUS_PATH_UNKNOWN;
    control_event_client_status(LOG_NOTICE, "NOT_ENOUGH_DIR_INFO");
  }
  have_min_dir_info = res;
  need_to_update_have_min_dir_info = 0;
}
