connection_edge_package_raw_inbuf(edge_connection_t *conn, int package_partial,
                                  int *max_cells)
{
  size_t bytes_to_process, length;
  char payload[CELL_PAYLOAD_SIZE];
  circuit_t *circ;
  const unsigned domain = conn->base_.type == CONN_TYPE_AP ? LD_APP : LD_EXIT;
  int sending_from_optimistic = 0;
  entry_connection_t *entry_conn =
    conn->base_.type == CONN_TYPE_AP ? EDGE_TO_ENTRY_CONN(conn) : NULL;
  const int sending_optimistically =
    entry_conn &&
    conn->base_.type == CONN_TYPE_AP &&
    conn->base_.state != AP_CONN_STATE_OPEN;
  crypt_path_t *cpath_layer = conn->cpath_layer;

  tor_assert(conn);

  if (conn->base_.marked_for_close) {
    log_warn(LD_BUG,
             "called on conn that's already marked for close at %s:%d.",
             conn->base_.marked_for_close_file, conn->base_.marked_for_close);
    return 0;
  }

  if (max_cells && *max_cells <= 0)
    return 0;

 repeat_connection_edge_package_raw_inbuf:

  circ = circuit_get_by_edge_conn(conn);
  if (!circ) {
    log_info(domain,"conn has no circuit! Closing.");
    conn->end_reason = END_STREAM_REASON_CANT_ATTACH;
    return -1;
  }

  if (circuit_consider_stop_edge_reading(circ, cpath_layer))
    return 0;

  if (conn->package_window <= 0) {
    log_info(domain,"called with package_window %d. Skipping.",
             conn->package_window);
    connection_stop_reading(TO_CONN(conn));
    return 0;
  }

  sending_from_optimistic = entry_conn &&
    entry_conn->sending_optimistic_data != NULL;

  if (PREDICT_UNLIKELY(sending_from_optimistic)) {
    bytes_to_process = generic_buffer_len(entry_conn->sending_optimistic_data);
    if (PREDICT_UNLIKELY(!bytes_to_process)) {
      log_warn(LD_BUG, "sending_optimistic_data was non-NULL but empty");
      bytes_to_process = connection_get_inbuf_len(TO_CONN(conn));
      sending_from_optimistic = 0;
    }
  } else {
    bytes_to_process = connection_get_inbuf_len(TO_CONN(conn));
  }

  if (!bytes_to_process)
    return 0;

  if (!package_partial && bytes_to_process < RELAY_PAYLOAD_SIZE)
    return 0;

  if (bytes_to_process > RELAY_PAYLOAD_SIZE) {
    length = RELAY_PAYLOAD_SIZE;
  } else {
    length = bytes_to_process;
  }
  stats_n_data_bytes_packaged += length;
  stats_n_data_cells_packaged += 1;

  if (PREDICT_UNLIKELY(sending_from_optimistic)) {
    /* XXXX We could be more efficient here by sometimes packing
     * previously-sent optimistic data in the same cell with data
     * from the inbuf. */
    generic_buffer_get(entry_conn->sending_optimistic_data, payload, length);
    if (!generic_buffer_len(entry_conn->sending_optimistic_data)) {
        generic_buffer_free(entry_conn->sending_optimistic_data);
        entry_conn->sending_optimistic_data = NULL;
    }
  } else {
    connection_fetch_from_buf(payload, length, TO_CONN(conn));
  }

  log_debug(domain,TOR_SOCKET_T_FORMAT": Packaging %d bytes (%d waiting).",
            conn->base_.s,
            (int)length, (int)connection_get_inbuf_len(TO_CONN(conn)));

  if (sending_optimistically && !sending_from_optimistic) {
    /* This is new optimistic data; remember it in case we need to detach and
       retry */
    if (!entry_conn->pending_optimistic_data)
      entry_conn->pending_optimistic_data = generic_buffer_new();
    generic_buffer_add(entry_conn->pending_optimistic_data, payload, length);
  }

  if (connection_edge_send_command(conn, RELAY_COMMAND_DATA,
                                   payload, length) < 0 )
    /* circuit got marked for close, don't continue, don't need to mark conn */
    return 0;

  if (!cpath_layer) { /* non-rendezvous exit */
    tor_assert(circ->package_window > 0);
    circ->package_window--;
  } else { /* we're an AP, or an exit on a rendezvous circ */
    tor_assert(cpath_layer->package_window > 0);
    cpath_layer->package_window--;
  }

  if (--conn->package_window <= 0) { /* is it 0 after decrement? */
    connection_stop_reading(TO_CONN(conn));
    log_debug(domain,"conn->package_window reached 0.");
    circuit_consider_stop_edge_reading(circ, cpath_layer);
    return 0; /* don't process the inbuf any more */
  }
  log_debug(domain,"conn->package_window is now %d",conn->package_window);

  if (max_cells) {
    *max_cells -= 1;
    if (*max_cells <= 0)
      return 0;
  }

  /* handle more if there's more, or return 0 if there isn't */
  goto repeat_connection_edge_package_raw_inbuf;
}
