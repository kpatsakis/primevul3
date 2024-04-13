free_cell_pool(void)
{
  /* Maybe we haven't called init_cell_pool yet; need to check for it. */
  if (cell_pool) {
    mp_pool_destroy(cell_pool);
    cell_pool = NULL;
  }
}
