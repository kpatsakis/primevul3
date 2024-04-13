cell_queues_check_size(void)
{
  size_t alloc = total_cells_allocated * packed_cell_mem_cost();
  if (alloc >= get_options()->MaxMemInCellQueues) {
    circuits_handle_oom(alloc);
    return 1;
  }
  return 0;
}
