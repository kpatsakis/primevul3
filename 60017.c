sampled_guards_update_from_consensus(guard_selection_t *gs)
{
  tor_assert(gs);
  const int REMOVE_UNLISTED_GUARDS_AFTER =
    (get_remove_unlisted_guards_after_days() * 86400);
  const int unlisted_since_slop = REMOVE_UNLISTED_GUARDS_AFTER / 5;

  if (live_consensus_is_missing(gs)) {
    log_info(LD_GUARD, "Not updating the sample guard set; we have "
             "no live consensus.");
    return;
  }
  log_info(LD_GUARD, "Updating sampled guard status based on received "
           "consensus.");

  int n_changes = 0;

  /* First: Update listed/unlisted. */
  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    /* XXXX #20827 check ed ID too */
    const int is_listed = entry_guard_is_listed(gs, guard);

    if (is_listed && ! guard->currently_listed) {
      ++n_changes;
      guard->currently_listed = 1;
      guard->unlisted_since_date = 0;
      log_info(LD_GUARD, "Sampled guard %s is now listed again.",
               entry_guard_describe(guard));
    } else if (!is_listed && guard->currently_listed) {
      ++n_changes;
      guard->currently_listed = 0;
      guard->unlisted_since_date = randomize_time(approx_time(),
                                                  unlisted_since_slop);
      log_info(LD_GUARD, "Sampled guard %s is now unlisted.",
               entry_guard_describe(guard));
    } else if (is_listed && guard->currently_listed) {
      log_debug(LD_GUARD, "Sampled guard %s is still listed.",
               entry_guard_describe(guard));
    } else {
      tor_assert(! is_listed && ! guard->currently_listed);
      log_debug(LD_GUARD, "Sampled guard %s is still unlisted.",
                entry_guard_describe(guard));
    }

    /* Clean up unlisted_since_date, just in case. */
    if (guard->currently_listed && guard->unlisted_since_date) {
      ++n_changes;
      guard->unlisted_since_date = 0;
      log_warn(LD_BUG, "Sampled guard %s was listed, but with "
               "unlisted_since_date set. Fixing.",
               entry_guard_describe(guard));
    } else if (!guard->currently_listed && ! guard->unlisted_since_date) {
      ++n_changes;
      guard->unlisted_since_date = randomize_time(approx_time(),
                                                  unlisted_since_slop);
      log_warn(LD_BUG, "Sampled guard %s was unlisted, but with "
               "unlisted_since_date unset. Fixing.",
               entry_guard_describe(guard));
    }
  } SMARTLIST_FOREACH_END(guard);

  const time_t remove_if_unlisted_since =
    approx_time() - REMOVE_UNLISTED_GUARDS_AFTER;
  const time_t maybe_remove_if_sampled_before =
    approx_time() - get_guard_lifetime();
  const time_t remove_if_confirmed_before =
    approx_time() - get_guard_confirmed_min_lifetime();

  /* Then: remove the ones that have been junk for too long */
  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    int rmv = 0;

    if (guard->currently_listed == 0 &&
        guard->unlisted_since_date < remove_if_unlisted_since) {
      /*
        "We have a live consensus, and {IS_LISTED} is false, and
         {FIRST_UNLISTED_AT} is over {REMOVE_UNLISTED_GUARDS_AFTER}
         days in the past."
      */
      log_info(LD_GUARD, "Removing sampled guard %s: it has been unlisted "
               "for over %d days", entry_guard_describe(guard),
               get_remove_unlisted_guards_after_days());
      rmv = 1;
    } else if (guard->sampled_on_date < maybe_remove_if_sampled_before) {
      /* We have a live consensus, and {ADDED_ON_DATE} is over
        {GUARD_LIFETIME} ago, *and* {CONFIRMED_ON_DATE} is either
        "never", or over {GUARD_CONFIRMED_MIN_LIFETIME} ago.
      */
      if (guard->confirmed_on_date == 0) {
        rmv = 1;
        log_info(LD_GUARD, "Removing sampled guard %s: it was sampled "
                 "over %d days ago, but never confirmed.",
                 entry_guard_describe(guard),
                 get_guard_lifetime() / 86400);
      } else if (guard->confirmed_on_date < remove_if_confirmed_before) {
        rmv = 1;
        log_info(LD_GUARD, "Removing sampled guard %s: it was sampled "
                 "over %d days ago, and confirmed over %d days ago.",
                 entry_guard_describe(guard),
                 get_guard_lifetime() / 86400,
                 get_guard_confirmed_min_lifetime() / 86400);
      }
    }

    if (rmv) {
      ++n_changes;
      SMARTLIST_DEL_CURRENT(gs->sampled_entry_guards, guard);
      remove_guard_from_confirmed_and_primary_lists(gs, guard);
      entry_guard_free(guard);
    }
  } SMARTLIST_FOREACH_END(guard);

  if (n_changes) {
    gs->primary_guards_up_to_date = 0;
    entry_guards_update_filtered_sets(gs);
    /* We don't need to rebuild the confirmed list right here -- we may have
     * removed confirmed guards above, but we can't have added any new
     * confirmed guards.
     */
    entry_guards_changed_for_guard_selection(gs);
  }
}
