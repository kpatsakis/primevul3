circuit_resume_edge_reading_helper(edge_connection_t *first_conn,
                                   circuit_t *circ,
                                   crypt_path_t *layer_hint)
{
  edge_connection_t *conn;
  int n_packaging_streams, n_streams_left;
  int packaged_this_round;
  int cells_on_queue;
  int cells_per_conn;
  edge_connection_t *chosen_stream = NULL;
  int max_to_package;

  if (first_conn == NULL) {
    /* Don't bother to try to do the rest of this if there are no connections
     * to resume. */
    return 0;
  }

  /* How many cells do we have space for?  It will be the minimum of
   * the number needed to exhaust the package window, and the minimum
   * needed to fill the cell queue. */
  max_to_package = circ->package_window;
  if (CIRCUIT_IS_ORIGIN(circ)) {
    cells_on_queue = circ->n_chan_cells.n;
  } else {
    or_circuit_t *or_circ = TO_OR_CIRCUIT(circ);
    cells_on_queue = or_circ->p_chan_cells.n;
  }
  if (CELL_QUEUE_HIGHWATER_SIZE - cells_on_queue < max_to_package)
    max_to_package = CELL_QUEUE_HIGHWATER_SIZE - cells_on_queue;

  /* Once we used to start listening on the streams in the order they
   * appeared in the linked list.  That leads to starvation on the
   * streams that appeared later on the list, since the first streams
   * would always get to read first.  Instead, we just pick a random
   * stream on the list, and enable reading for streams starting at that
   * point (and wrapping around as if the list were circular).  It would
   * probably be better to actually remember which streams we've
   * serviced in the past, but this is simple and effective. */

  /* Select a stream uniformly at random from the linked list.  We
   * don't need cryptographic randomness here. */
  {
    int num_streams = 0;
    for (conn = first_conn; conn; conn = conn->next_stream) {
      num_streams++;
      if (tor_weak_random_one_in_n(&stream_choice_rng, num_streams)) {
        chosen_stream = conn;
      }
      /* Invariant: chosen_stream has been chosen uniformly at random from
       * among the first num_streams streams on first_conn.
       *
       * (Note that we iterate over every stream on the circuit, so that after
       * we've considered the first stream, we've chosen it with P=1; and
       * after we consider the second stream, we've switched to it with P=1/2
       * and stayed with the first stream with P=1/2; and after we've
       * considered the third stream, we've switched to it with P=1/3 and
       * remained with one of the first two streams with P=(2/3), giving each
       * one P=(1/2)(2/3) )=(1/3).) */
    }
  }

  /* Count how many non-marked streams there are that have anything on
   * their inbuf, and enable reading on all of the connections. */
  n_packaging_streams = 0;
  /* Activate reading starting from the chosen stream */
  for (conn=chosen_stream; conn; conn = conn->next_stream) {
    /* Start reading for the streams starting from here */
    if (conn->base_.marked_for_close || conn->package_window <= 0)
      continue;
    if (!layer_hint || conn->cpath_layer == layer_hint) {
      connection_start_reading(TO_CONN(conn));

      if (connection_get_inbuf_len(TO_CONN(conn)) > 0)
        ++n_packaging_streams;
    }
  }
  /* Go back and do the ones we skipped, circular-style */
  for (conn = first_conn; conn != chosen_stream; conn = conn->next_stream) {
    if (conn->base_.marked_for_close || conn->package_window <= 0)
      continue;
    if (!layer_hint || conn->cpath_layer == layer_hint) {
      connection_start_reading(TO_CONN(conn));

      if (connection_get_inbuf_len(TO_CONN(conn)) > 0)
        ++n_packaging_streams;
    }
  }

  if (n_packaging_streams == 0) /* avoid divide-by-zero */
    return 0;

 again:

  cells_per_conn = CEIL_DIV(max_to_package, n_packaging_streams);

  packaged_this_round = 0;
  n_streams_left = 0;

  /* Iterate over all connections.  Package up to cells_per_conn cells on
   * each.  Update packaged_this_round with the total number of cells
   * packaged, and n_streams_left with the number that still have data to
   * package.
   */
  for (conn=first_conn; conn; conn=conn->next_stream) {
    if (conn->base_.marked_for_close || conn->package_window <= 0)
      continue;
    if (!layer_hint || conn->cpath_layer == layer_hint) {
      int n = cells_per_conn, r;
      /* handle whatever might still be on the inbuf */
      r = connection_edge_package_raw_inbuf(conn, 1, &n);

      /* Note how many we packaged */
      packaged_this_round += (cells_per_conn-n);

      if (r<0) {
        /* Problem while packaging. (We already sent an end cell if
         * possible) */
        connection_mark_for_close(TO_CONN(conn));
        continue;
      }

      /* If there's still data to read, we'll be coming back to this stream. */
      if (connection_get_inbuf_len(TO_CONN(conn)))
          ++n_streams_left;

      /* If the circuit won't accept any more data, return without looking
       * at any more of the streams. Any connections that should be stopped
       * have already been stopped by connection_edge_package_raw_inbuf. */
      if (circuit_consider_stop_edge_reading(circ, layer_hint))
        return -1;
      /* XXXX should we also stop immediately if we fill up the cell queue?
       * Probably. */
    }
  }

  /* If we made progress, and we are willing to package more, and there are
   * any streams left that want to package stuff... try again!
   */
  if (packaged_this_round && packaged_this_round < max_to_package &&
      n_streams_left) {
    max_to_package -= packaged_this_round;
    n_packaging_streams = n_streams_left;
    goto again;
  }

  return 0;
}
