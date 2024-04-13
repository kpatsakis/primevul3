cell_queue_append_packed_copy(cell_queue_t *queue, const cell_t *cell,
                              int wide_circ_ids)
{
  struct timeval now;
  packed_cell_t *copy = packed_cell_copy(cell, wide_circ_ids);
  tor_gettimeofday_cached(&now);
  copy->inserted_time = (uint32_t)tv_to_msec(&now);

  cell_queue_append(queue, copy);
}
