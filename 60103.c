circuit_consider_stop_edge_reading(circuit_t *circ, crypt_path_t *layer_hint)
{
  edge_connection_t *conn = NULL;
  unsigned domain = layer_hint ? LD_APP : LD_EXIT;

  if (!layer_hint) {
    or_circuit_t *or_circ = TO_OR_CIRCUIT(circ);
    log_debug(domain,"considering circ->package_window %d",
              circ->package_window);
    if (circ->package_window <= 0) {
      log_debug(domain,"yes, not-at-origin. stopped.");
      for (conn = or_circ->n_streams; conn; conn=conn->next_stream)
        connection_stop_reading(TO_CONN(conn));
      return 1;
    }
    return 0;
  }
  /* else, layer hint is defined, use it */
  log_debug(domain,"considering layer_hint->package_window %d",
            layer_hint->package_window);
  if (layer_hint->package_window <= 0) {
    log_debug(domain,"yes, at-origin. stopped.");
    for (conn = TO_ORIGIN_CIRCUIT(circ)->p_streams; conn;
         conn=conn->next_stream) {
      if (conn->cpath_layer == layer_hint)
        connection_stop_reading(TO_CONN(conn));
    }
    return 1;
  }
  return 0;
}
