MODRET auth_post_host(cmd_rec *cmd) {

  /* If the HOST command changed the main_server pointer, reinitialize
   * ourselves.
   */
  if (session.prev_server != NULL) {
    int res;

    /* Remove the TimeoutLogin timer. */
    pr_timer_remove(PR_TIMER_LOGIN, &auth_module);

    pr_event_unregister(&auth_module, "core.exit", auth_exit_ev);

    /* Reset the CreateHome setting. */
    mkhome = FALSE;

#ifdef PR_USE_LASTLOG
    /* Reset the UseLastLog setting. */
    lastlog = FALSE;
#endif /* PR_USE_LASTLOG */

    res = auth_sess_init();
    if (res < 0) {
      pr_session_disconnect(&auth_module,
        PR_SESS_DISCONNECT_SESSION_INIT_FAILED, NULL);
    }
  }

  return PR_DECLINED(cmd);
}
