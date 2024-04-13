relay_send_command_from_edge_(streamid_t stream_id, circuit_t *circ,
                              uint8_t relay_command, const char *payload,
                              size_t payload_len, crypt_path_t *cpath_layer,
                              const char *filename, int lineno)
{
  cell_t cell;
  relay_header_t rh;
  cell_direction_t cell_direction;
  /* XXXX NM Split this function into a separate versions per circuit type? */

  tor_assert(circ);
  tor_assert(payload_len <= RELAY_PAYLOAD_SIZE);

  memset(&cell, 0, sizeof(cell_t));
  cell.command = CELL_RELAY;
  if (cpath_layer) {
    cell.circ_id = circ->n_circ_id;
    cell_direction = CELL_DIRECTION_OUT;
  } else if (! CIRCUIT_IS_ORIGIN(circ)) {
    cell.circ_id = TO_OR_CIRCUIT(circ)->p_circ_id;
    cell_direction = CELL_DIRECTION_IN;
  } else {
    return -1;
  }

  memset(&rh, 0, sizeof(rh));
  rh.command = relay_command;
  rh.stream_id = stream_id;
  rh.length = payload_len;
  relay_header_pack(cell.payload, &rh);
  if (payload_len)
    memcpy(cell.payload+RELAY_HEADER_SIZE, payload, payload_len);

  log_debug(LD_OR,"delivering %d cell %s.", relay_command,
            cell_direction == CELL_DIRECTION_OUT ? "forward" : "backward");

  /* If we are sending an END cell and this circuit is used for a tunneled
   * directory request, advance its state. */
  if (relay_command == RELAY_COMMAND_END && circ->dirreq_id)
    geoip_change_dirreq_state(circ->dirreq_id, DIRREQ_TUNNELED,
                              DIRREQ_END_CELL_SENT);

  if (cell_direction == CELL_DIRECTION_OUT && circ->n_chan) {
    /* if we're using relaybandwidthrate, this conn wants priority */
    channel_timestamp_client(circ->n_chan);
  }

  if (cell_direction == CELL_DIRECTION_OUT) {
    origin_circuit_t *origin_circ = TO_ORIGIN_CIRCUIT(circ);
    if (origin_circ->remaining_relay_early_cells > 0 &&
        (relay_command == RELAY_COMMAND_EXTEND ||
         relay_command == RELAY_COMMAND_EXTEND2 ||
         cpath_layer != origin_circ->cpath)) {
      /* If we've got any relay_early cells left and (we're sending
       * an extend cell or we're not talking to the first hop), use
       * one of them.  Don't worry about the conn protocol version:
       * append_cell_to_circuit_queue will fix it up. */
      cell.command = CELL_RELAY_EARLY;
      --origin_circ->remaining_relay_early_cells;
      log_debug(LD_OR, "Sending a RELAY_EARLY cell; %d remaining.",
                (int)origin_circ->remaining_relay_early_cells);
      /* Memorize the command that is sent as RELAY_EARLY cell; helps debug
       * task 878. */
      origin_circ->relay_early_commands[
          origin_circ->relay_early_cells_sent++] = relay_command;
    } else if (relay_command == RELAY_COMMAND_EXTEND ||
               relay_command == RELAY_COMMAND_EXTEND2) {
      /* If no RELAY_EARLY cells can be sent over this circuit, log which
       * commands have been sent as RELAY_EARLY cells before; helps debug
       * task 878. */
      smartlist_t *commands_list = smartlist_new();
      int i = 0;
      char *commands = NULL;
      for (; i < origin_circ->relay_early_cells_sent; i++)
        smartlist_add(commands_list, (char *)
            relay_command_to_string(origin_circ->relay_early_commands[i]));
      commands = smartlist_join_strings(commands_list, ",", 0, NULL);
      log_warn(LD_BUG, "Uh-oh.  We're sending a RELAY_COMMAND_EXTEND cell, "
               "but we have run out of RELAY_EARLY cells on that circuit. "
               "Commands sent before: %s", commands);
      tor_free(commands);
      smartlist_free(commands_list);
    }
  }

  if (circuit_package_relay_cell(&cell, circ, cell_direction, cpath_layer,
                                 stream_id, filename, lineno) < 0) {
    log_warn(LD_BUG,"circuit_package_relay_cell failed. Closing.");
    circuit_mark_for_close(circ, END_CIRC_REASON_INTERNAL);
    return -1;
  }
  return 0;
}
