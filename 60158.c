connection_ap_rewrite_and_attach_if_allowed(entry_connection_t *conn,
                                            origin_circuit_t *circ,
                                            crypt_path_t *cpath)
{
  const or_options_t *options = get_options();

  if (options->LeaveStreamsUnattached) {
    CONNECTION_AP_EXPECT_NONPENDING(conn);
    ENTRY_TO_CONN(conn)->state = AP_CONN_STATE_CONTROLLER_WAIT;
    return 0;
  }
  return connection_ap_handshake_rewrite_and_attach(conn, circ, cpath);
}
