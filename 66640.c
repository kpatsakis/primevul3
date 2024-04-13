mrb_define_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid, mrb_func_t func, mrb_aspec aspec)
{
  mrb_method_t m;
  int ai = mrb_gc_arena_save(mrb);

  MRB_METHOD_FROM_FUNC(m, func);
  mrb_define_method_raw(mrb, c, mid, m);
  mrb_gc_arena_restore(mrb, ai);
}
