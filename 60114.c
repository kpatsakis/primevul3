dump_cell_pool_usage(int severity)
{
  circuit_t *c;
  int n_circs = 0;
  int n_cells = 0;
  for (c = circuit_get_global_list_(); c; c = c->next) {
    n_cells += c->n_chan_cells.n;
    if (!CIRCUIT_IS_ORIGIN(c))
      n_cells += TO_OR_CIRCUIT(c)->p_chan_cells.n;
    ++n_circs;
  }
  tor_log(severity, LD_MM,
          "%d cells allocated on %d circuits. %d cells leaked.",
          n_cells, n_circs, (int)total_cells_allocated - n_cells);
  mp_pool_log_status(cell_pool, severity);
}
