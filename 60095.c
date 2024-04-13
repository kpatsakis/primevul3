cell_queue_append(cell_queue_t *queue, packed_cell_t *cell)
{
  if (queue->tail) {
    tor_assert(!queue->tail->next);
    queue->tail->next = cell;
  } else {
    queue->head = cell;
  }
  queue->tail = cell;
  cell->next = NULL;
  ++queue->n;
}
