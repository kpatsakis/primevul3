circuit_receive_relay_cell(cell_t *cell, circuit_t *circ,
                           cell_direction_t cell_direction)
{
  channel_t *chan = NULL;
  crypt_path_t *layer_hint=NULL;
  char recognized=0;
  int reason;

  tor_assert(cell);
  tor_assert(circ);
  tor_assert(cell_direction == CELL_DIRECTION_OUT ||
             cell_direction == CELL_DIRECTION_IN);
  if (circ->marked_for_close)
    return 0;

  if (relay_crypt(circ, cell, cell_direction, &layer_hint, &recognized) < 0) {
    log_warn(LD_BUG,"relay crypt failed. Dropping connection.");
    return -END_CIRC_REASON_INTERNAL;
  }

  if (recognized) {
    edge_connection_t *conn = NULL;

    if (circ->purpose == CIRCUIT_PURPOSE_PATH_BIAS_TESTING) {
      pathbias_check_probe_response(circ, cell);

      /* We need to drop this cell no matter what to avoid code that expects
       * a certain purpose (such as the hidserv code). */
      return 0;
    }

    conn = relay_lookup_conn(circ, cell, cell_direction,
                                                layer_hint);
    if (cell_direction == CELL_DIRECTION_OUT) {
      ++stats_n_relay_cells_delivered;
      log_debug(LD_OR,"Sending away from origin.");
      if ((reason=connection_edge_process_relay_cell(cell, circ, conn, NULL))
          < 0) {
        log_fn(LOG_PROTOCOL_WARN, LD_PROTOCOL,
               "connection_edge_process_relay_cell (away from origin) "
               "failed.");
        return reason;
      }
    }
    if (cell_direction == CELL_DIRECTION_IN) {
      ++stats_n_relay_cells_delivered;
      log_debug(LD_OR,"Sending to origin.");
      if ((reason = connection_edge_process_relay_cell(cell, circ, conn,
                                                       layer_hint)) < 0) {
        log_warn(LD_OR,
                 "connection_edge_process_relay_cell (at origin) failed.");
        return reason;
      }
    }
    return 0;
  }

  /* not recognized. pass it on. */
  if (cell_direction == CELL_DIRECTION_OUT) {
    cell->circ_id = circ->n_circ_id; /* switch it */
    chan = circ->n_chan;
  } else if (! CIRCUIT_IS_ORIGIN(circ)) {
    cell->circ_id = TO_OR_CIRCUIT(circ)->p_circ_id; /* switch it */
    chan = TO_OR_CIRCUIT(circ)->p_chan;
  } else {
    log_fn(LOG_PROTOCOL_WARN, LD_OR,
           "Dropping unrecognized inbound cell on origin circuit.");
    /* If we see unrecognized cells on path bias testing circs,
     * it's bad mojo. Those circuits need to die.
     * XXX: Shouldn't they always die? */
    if (circ->purpose == CIRCUIT_PURPOSE_PATH_BIAS_TESTING) {
      TO_ORIGIN_CIRCUIT(circ)->path_state = PATH_STATE_USE_FAILED;
      return -END_CIRC_REASON_TORPROTOCOL;
    } else {
      return 0;
    }
  }

  if (!chan) {
    if (! CIRCUIT_IS_ORIGIN(circ) &&
        TO_OR_CIRCUIT(circ)->rend_splice &&
        cell_direction == CELL_DIRECTION_OUT) {
      or_circuit_t *splice = TO_OR_CIRCUIT(circ)->rend_splice;
      tor_assert(circ->purpose == CIRCUIT_PURPOSE_REND_ESTABLISHED);
      tor_assert(splice->base_.purpose == CIRCUIT_PURPOSE_REND_ESTABLISHED);
      cell->circ_id = splice->p_circ_id;
      cell->command = CELL_RELAY; /* can't be relay_early anyway */
      if ((reason = circuit_receive_relay_cell(cell, TO_CIRCUIT(splice),
                                               CELL_DIRECTION_IN)) < 0) {
        log_warn(LD_REND, "Error relaying cell across rendezvous; closing "
                 "circuits");
        /* XXXX Do this here, or just return -1? */
        circuit_mark_for_close(circ, -reason);
        return reason;
      }
      return 0;
    }
    log_fn(LOG_PROTOCOL_WARN, LD_PROTOCOL,
           "Didn't recognize cell, but circ stops here! Closing circ.");
    return -END_CIRC_REASON_TORPROTOCOL;
  }

  log_debug(LD_OR,"Passing on unrecognized cell.");

  ++stats_n_relay_cells_relayed; /* XXXX no longer quite accurate {cells}
                                  * we might kill the circ before we relay
                                  * the cells. */

  append_cell_to_circuit_queue(circ, chan, cell, cell_direction, 0);
  return 0;
}
