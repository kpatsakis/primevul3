channel_flush_from_first_active_circuit(channel_t *chan, int max)
{
  circuitmux_t *cmux = NULL;
  int n_flushed = 0;
  cell_queue_t *queue;
  circuit_t *circ;
  or_circuit_t *or_circ;
  int streams_blocked;
  packed_cell_t *cell;

  /* Get the cmux */
  tor_assert(chan);
  tor_assert(chan->cmux);
  cmux = chan->cmux;

  /* Main loop: pick a circuit, send a cell, update the cmux */
  while (n_flushed < max) {
    circ = circuitmux_get_first_active_circuit(cmux);
    /* If it returns NULL, no cells left to send */
    if (!circ) break;
    assert_cmux_ok_paranoid(chan);

    if (circ->n_chan == chan) {
      queue = &circ->n_chan_cells;
      streams_blocked = circ->streams_blocked_on_n_chan;
    } else {
      or_circ = TO_OR_CIRCUIT(circ);
      tor_assert(or_circ->p_chan == chan);
      queue = &TO_OR_CIRCUIT(circ)->p_chan_cells;
      streams_blocked = circ->streams_blocked_on_p_chan;
    }

    /* Circuitmux told us this was active, so it should have cells */
    tor_assert(queue->n > 0);

    /*
     * Get just one cell here; once we've sent it, that can change the circuit
     * selection, so we have to loop around for another even if this circuit
     * has more than one.
     */
    cell = cell_queue_pop(queue);

    /* Calculate the exact time that this cell has spent in the queue. */
    if (get_options()->CellStatistics && !CIRCUIT_IS_ORIGIN(circ)) {
      uint32_t msec_waiting;
      struct timeval tvnow;
      or_circ = TO_OR_CIRCUIT(circ);
      tor_gettimeofday_cached(&tvnow);
      msec_waiting = ((uint32_t)tv_to_msec(&tvnow)) - cell->inserted_time;

      or_circ->total_cell_waiting_time += msec_waiting;
      or_circ->processed_cells++;
    }

    /* If we just flushed our queue and this circuit is used for a
     * tunneled directory request, possibly advance its state. */
    if (queue->n == 0 && chan->dirreq_id)
      geoip_change_dirreq_state(chan->dirreq_id,
                                DIRREQ_TUNNELED,
                                DIRREQ_CIRC_QUEUE_FLUSHED);

    /* Now send the cell */
    channel_write_packed_cell(chan, cell);
    cell = NULL;

    /*
     * Don't packed_cell_free_unchecked(cell) here because the channel will
     * do so when it gets out of the channel queue (probably already did, in
     * which case that was an immediate double-free bug).
     */

    /* Update the counter */
    ++n_flushed;

    /*
     * Now update the cmux; tell it we've just sent a cell, and how many
     * we have left.
     */
    circuitmux_notify_xmit_cells(cmux, circ, 1);
    circuitmux_set_num_cells(cmux, circ, queue->n);
    if (queue->n == 0)
      log_debug(LD_GENERAL, "Made a circuit inactive.");

    /* Is the cell queue low enough to unblock all the streams that are waiting
     * to write to this circuit? */
    if (streams_blocked && queue->n <= CELL_QUEUE_LOWWATER_SIZE)
      set_streams_blocked_on_circ(circ, chan, 0, 0); /* unblock streams */

    /* If n_flushed < max still, loop around and pick another circuit */
  }

  /* Okay, we're done sending now */
  assert_cmux_ok_paranoid(chan);

  return n_flushed;
}
