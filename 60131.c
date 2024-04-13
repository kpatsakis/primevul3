set_streams_blocked_on_circ(circuit_t *circ, channel_t *chan,
                            int block, streamid_t stream_id)
{
  edge_connection_t *edge = NULL;
  int n = 0;
  if (circ->n_chan == chan) {
    circ->streams_blocked_on_n_chan = block;
    if (CIRCUIT_IS_ORIGIN(circ))
      edge = TO_ORIGIN_CIRCUIT(circ)->p_streams;
  } else {
    circ->streams_blocked_on_p_chan = block;
    tor_assert(!CIRCUIT_IS_ORIGIN(circ));
    edge = TO_OR_CIRCUIT(circ)->n_streams;
  }

  for (; edge; edge = edge->next_stream) {
    connection_t *conn = TO_CONN(edge);
    if (stream_id && edge->stream_id != stream_id)
      continue;

    if (edge->edge_blocked_on_circ != block) {
      ++n;
      edge->edge_blocked_on_circ = block;
    }

    if (!conn->read_event && !HAS_BUFFEREVENT(conn)) {
      /* This connection is a placeholder for something; probably a DNS
       * request.  It can't actually stop or start reading.*/
      continue;
    }

    if (block) {
      if (connection_is_reading(conn))
        connection_stop_reading(conn);
    } else {
      /* Is this right? */
      if (!connection_is_reading(conn))
        connection_start_reading(conn);
    }
  }

  return n;
}
