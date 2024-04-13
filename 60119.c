packed_cell_mem_cost(void)
{
  return sizeof(packed_cell_t) + MP_POOL_ITEM_OVERHEAD;
}
