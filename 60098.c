cell_queue_pop(cell_queue_t *queue)
{
  packed_cell_t *cell = queue->head;
  if (!cell)
    return NULL;
  queue->head = cell->next;
  if (cell == queue->tail) {
    tor_assert(!queue->head);
    queue->tail = NULL;
  }
  --queue->n;
  return cell;
}
