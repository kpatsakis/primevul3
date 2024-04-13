router_have_minimum_dir_info(void)
{
  static int logged_delay=0;
  const char *delay_fetches_msg = NULL;
  if (should_delay_dir_fetches(get_options(), &delay_fetches_msg)) {
    if (!logged_delay)
      log_notice(LD_DIR, "Delaying directory fetches: %s", delay_fetches_msg);
    logged_delay=1;
    strlcpy(dir_info_status, delay_fetches_msg,  sizeof(dir_info_status));
    return 0;
  }
  logged_delay = 0; /* reset it if we get this far */

  if (PREDICT_UNLIKELY(need_to_update_have_min_dir_info)) {
    update_router_have_minimum_dir_info();
  }

  return have_min_dir_info;
}
