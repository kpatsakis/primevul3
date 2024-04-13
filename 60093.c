append_cell_to_circuit_queue(circuit_t *circ, channel_t *chan,
                             cell_t *cell, cell_direction_t direction,
                             streamid_t fromstream)
{
  or_circuit_t *orcirc = NULL;
  cell_queue_t *queue;
  int streams_blocked;

  if (circ->marked_for_close)
    return;

  if (direction == CELL_DIRECTION_OUT) {
    queue = &circ->n_chan_cells;
    streams_blocked = circ->streams_blocked_on_n_chan;
  } else {
    orcirc = TO_OR_CIRCUIT(circ);
    queue = &orcirc->p_chan_cells;
    streams_blocked = circ->streams_blocked_on_p_chan;
  }

  /*
   * Disabling this for now because of a possible guard discovery attack
   */
#if 0
  /* Are we a middle circuit about to exceed ORCIRC_MAX_MIDDLE_CELLS? */
  if ((circ->n_chan != NULL) && CIRCUIT_IS_ORCIRC(circ)) {
    orcirc = TO_OR_CIRCUIT(circ);
    if (orcirc->p_chan) {
      if (queue->n + 1 >= ORCIRC_MAX_MIDDLE_CELLS) {
        /* Queueing this cell would put queue over the cap */
        log_warn(LD_CIRC,
                 "Got a cell exceeding the cap of %u in the %s direction "
                 "on middle circ ID %u on chan ID " U64_FORMAT
                 "; killing the circuit.",
                 ORCIRC_MAX_MIDDLE_CELLS,
                 (direction == CELL_DIRECTION_OUT) ? "n" : "p",
                 (direction == CELL_DIRECTION_OUT) ?
                   circ->n_circ_id : orcirc->p_circ_id,
                 U64_PRINTF_ARG(
                   (direction == CELL_DIRECTION_OUT) ?
                      circ->n_chan->global_identifier :
                      orcirc->p_chan->global_identifier));
        circuit_mark_for_close(circ, END_CIRC_REASON_RESOURCELIMIT);
        return;
      }
    }
  }
#endif

  cell_queue_append_packed_copy(queue, cell, chan->wide_circ_ids);

  if (PREDICT_UNLIKELY(cell_queues_check_size())) {
    /* We ran the OOM handler */
    if (circ->marked_for_close)
      return;
  }

  /* If we have too many cells on the circuit, we should stop reading from
   * the edge streams for a while. */
  if (!streams_blocked && queue->n >= CELL_QUEUE_HIGHWATER_SIZE)
    set_streams_blocked_on_circ(circ, chan, 1, 0); /* block streams */

  if (streams_blocked && fromstream) {
    /* This edge connection is apparently not blocked; block it. */
    set_streams_blocked_on_circ(circ, chan, 1, fromstream);
  }

  update_circuit_on_cmux(circ, direction);
  if (queue->n == 1) {
    /* This was the first cell added to the queue.  We just made this
     * circuit active. */
    log_debug(LD_GENERAL, "Made a circuit active.");
  }

  if (!channel_has_queued_writes(chan)) {
    /* There is no data at all waiting to be sent on the outbuf.  Add a
     * cell, so that we can notice when it gets flushed, flushed_some can
     * get called, and we can start putting more data onto the buffer then.
     */
    log_debug(LD_GENERAL, "Primed a buffer.");
    channel_flush_from_first_active_circuit(chan, 1);
  }
}
