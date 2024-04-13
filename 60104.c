circuit_package_relay_cell(cell_t *cell, circuit_t *circ,
                           cell_direction_t cell_direction,
                           crypt_path_t *layer_hint, streamid_t on_stream,
                           const char *filename, int lineno)
{
  channel_t *chan; /* where to send the cell */

  if (cell_direction == CELL_DIRECTION_OUT) {
    crypt_path_t *thishop; /* counter for repeated crypts */
    chan = circ->n_chan;
    if (!chan) {
      log_warn(LD_BUG,"outgoing relay cell sent from %s:%d has n_chan==NULL."
               " Dropping.", filename, lineno);
      return 0; /* just drop it */
    }
    if (!CIRCUIT_IS_ORIGIN(circ)) {
      log_warn(LD_BUG,"outgoing relay cell sent from %s:%d on non-origin "
               "circ. Dropping.", filename, lineno);
      return 0; /* just drop it */
    }

    relay_set_digest(layer_hint->f_digest, cell);

    thishop = layer_hint;
    /* moving from farthest to nearest hop */
    do {
      tor_assert(thishop);
      /* XXXX RD This is a bug, right? */
      log_debug(LD_OR,"crypting a layer of the relay cell.");
      if (relay_crypt_one_payload(thishop->f_crypto, cell->payload, 1) < 0) {
        return -1;
      }

      thishop = thishop->prev;
    } while (thishop != TO_ORIGIN_CIRCUIT(circ)->cpath->prev);

  } else { /* incoming cell */
    or_circuit_t *or_circ;
    if (CIRCUIT_IS_ORIGIN(circ)) {
      /* We should never package an _incoming_ cell from the circuit
       * origin; that means we messed up somewhere. */
      log_warn(LD_BUG,"incoming relay cell at origin circuit. Dropping.");
      assert_circuit_ok(circ);
      return 0; /* just drop it */
    }
    or_circ = TO_OR_CIRCUIT(circ);
    chan = or_circ->p_chan;
    relay_set_digest(or_circ->p_digest, cell);
    if (relay_crypt_one_payload(or_circ->p_crypto, cell->payload, 1) < 0)
      return -1;
  }
  ++stats_n_relay_cells_relayed;

  append_cell_to_circuit_queue(circ, chan, cell, cell_direction, on_stream);
  return 0;
}
