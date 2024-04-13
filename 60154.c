connection_ap_mark_as_pending_circuit_(entry_connection_t *entry_conn,
                                       const char *fname, int lineno)
{
  connection_t *conn = ENTRY_TO_CONN(entry_conn);
  tor_assert(conn->state == AP_CONN_STATE_CIRCUIT_WAIT);
  tor_assert(conn->magic == ENTRY_CONNECTION_MAGIC);
  if (conn->marked_for_close)
    return;

  if (PREDICT_UNLIKELY(NULL == pending_entry_connections))
    pending_entry_connections = smartlist_new();

  if (PREDICT_UNLIKELY(smartlist_contains(pending_entry_connections,
                                          entry_conn))) {
    log_warn(LD_BUG, "What?? pending_entry_connections already contains %p! "
             "(Called from %s:%d.)",
             entry_conn, fname, lineno);
#ifdef DEBUGGING_17659
    const char *f2 = entry_conn->marked_pending_circ_file;
    log_warn(LD_BUG, "(Previously called from %s:%d.)\n",
             f2 ? f2 : "<NULL>",
             entry_conn->marked_pending_circ_line);
#endif
    log_backtrace(LOG_WARN, LD_BUG, "To debug, this may help");
    return;
  }

#ifdef DEBUGGING_17659
  entry_conn->marked_pending_circ_line = (uint16_t) lineno;
  entry_conn->marked_pending_circ_file = fname;
#endif

  untried_pending_connections = 1;
  smartlist_add(pending_entry_connections, entry_conn);

  /* Work-around for bug 19969: we handle pending_entry_connections at
   * the end of run_main_loop_once(), but in many cases that function will
   * take a very long time, if ever, to finish its call to event_base_loop().
   *
   * So the fix is to tell it right now that it ought to finish its loop at
   * its next available opportunity.
   */
  tell_event_loop_to_finish();
}
