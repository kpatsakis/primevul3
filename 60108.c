clean_cell_pool(void)
{
  tor_assert(cell_pool);
  mp_pool_clean(cell_pool, 0, 1);
}
