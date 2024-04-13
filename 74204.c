static void auth_exit_ev(const void *event_data, void *user_data) {
  /* Close the scoreboard descriptor that we opened. */
  (void) pr_close_scoreboard(FALSE);
}
