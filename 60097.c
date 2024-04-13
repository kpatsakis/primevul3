cell_queue_clear(cell_queue_t *queue)
{
  packed_cell_t *cell, *next;
  cell = queue->head;
  while (cell) {
    next = cell->next;
    packed_cell_free_unchecked(cell);
    cell = next;
  }
  queue->head = queue->tail = NULL;
  queue->n = 0;
}
