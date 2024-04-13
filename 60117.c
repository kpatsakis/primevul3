packed_cell_copy(const cell_t *cell, int wide_circ_ids)
{
  packed_cell_t *c = packed_cell_new();
  cell_pack(c, cell, wide_circ_ids);
  c->next = NULL;
  return c;
}
