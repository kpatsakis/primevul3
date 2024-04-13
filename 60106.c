circuit_resume_edge_reading(circuit_t *circ, crypt_path_t *layer_hint)
{
  if (circuit_queue_streams_are_blocked(circ)) {
    log_debug(layer_hint?LD_APP:LD_EXIT,"Too big queue, no resuming");
    return;
  }
  log_debug(layer_hint?LD_APP:LD_EXIT,"resuming");

  if (CIRCUIT_IS_ORIGIN(circ))
    circuit_resume_edge_reading_helper(TO_ORIGIN_CIRCUIT(circ)->p_streams,
                                       circ, layer_hint);
  else
    circuit_resume_edge_reading_helper(TO_OR_CIRCUIT(circ)->n_streams,
                                       circ, layer_hint);
}
