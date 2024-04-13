packed_cell_new(void)
{
  ++total_cells_allocated;
  return mp_pool_get(cell_pool);
}
