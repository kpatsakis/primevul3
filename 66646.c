mrb_gc_free_mt(mrb_state *mrb, struct RClass *c)
{
  kh_destroy(mt, mrb, c->mt);
}
