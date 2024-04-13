mc_clear_all(mrb_state *mrb)
{
  struct mrb_cache_entry *mc = mrb->cache;
  int i;

  for (i=0; i<MRB_METHOD_CACHE_SIZE; i++) {
    mc[i].c = 0;
  }
}
