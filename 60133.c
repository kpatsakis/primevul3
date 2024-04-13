update_circuit_on_cmux_(circuit_t *circ, cell_direction_t direction,
                        const char *file, int lineno)
{
  channel_t *chan = NULL;
  or_circuit_t *or_circ = NULL;
  circuitmux_t *cmux = NULL;

  tor_assert(circ);

  /* Okay, get the channel */
  if (direction == CELL_DIRECTION_OUT) {
    chan = circ->n_chan;
  } else {
    or_circ = TO_OR_CIRCUIT(circ);
    chan = or_circ->p_chan;
  }

  tor_assert(chan);
  tor_assert(chan->cmux);

  /* Now get the cmux */
  cmux = chan->cmux;

  /* Cmux sanity check */
  if (! circuitmux_is_circuit_attached(cmux, circ)) {
    log_warn(LD_BUG, "called on non-attachd circuit from %s:%d",
             file, lineno);
    return;
  }
  tor_assert(circuitmux_attached_circuit_direction(cmux, circ) == direction);

  assert_cmux_ok_paranoid(chan);

  /* Update the number of cells we have for the circuit mux */
  if (direction == CELL_DIRECTION_OUT) {
    circuitmux_set_num_cells(cmux, circ, circ->n_chan_cells.n);
  } else {
    circuitmux_set_num_cells(cmux, circ, or_circ->p_chan_cells.n);
  }

  assert_cmux_ok_paranoid(chan);
}
