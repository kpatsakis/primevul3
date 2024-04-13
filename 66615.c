mc_clear_by_id(mrb_state *mrb, struct RClass *c, mrb_sym mid)
{
  struct mrb_cache_entry *mc = mrb->cache;
  int i;

  if (c->flags & MRB_FLAG_IS_INHERITED) {
    mc_clear_all(mrb);
    c->flags &= ~MRB_FLAG_IS_INHERITED;
    return;
  }
  for (i=0; i<MRB_METHOD_CACHE_SIZE; i++) {
    if (mc[i].c == c || mc[i].mid == mid)
      mc[i].c = 0;
  }
}
