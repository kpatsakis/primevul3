connection_edge_consider_sending_sendme(edge_connection_t *conn)
{
  circuit_t *circ;

  if (connection_outbuf_too_full(TO_CONN(conn)))
    return;

  circ = circuit_get_by_edge_conn(conn);
  if (!circ) {
    /* this can legitimately happen if the destroy has already
     * arrived and torn down the circuit */
    log_info(LD_APP,"No circuit associated with conn. Skipping.");
    return;
  }

  while (conn->deliver_window <= STREAMWINDOW_START - STREAMWINDOW_INCREMENT) {
    log_debug(conn->base_.type == CONN_TYPE_AP ?LD_APP:LD_EXIT,
              "Outbuf %d, Queuing stream sendme.",
              (int)conn->base_.outbuf_flushlen);
    conn->deliver_window += STREAMWINDOW_INCREMENT;
    if (connection_edge_send_command(conn, RELAY_COMMAND_SENDME,
                                     NULL, 0) < 0) {
      log_warn(LD_APP,"connection_edge_send_command failed. Skipping.");
      return; /* the circuit's closed, don't continue */
    }
  }
}
