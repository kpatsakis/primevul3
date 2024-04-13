mrb_alias_method(mrb_state *mrb, struct RClass *c, mrb_sym a, mrb_sym b)
{
  mrb_method_t m = mrb_method_search(mrb, c, b);

  mrb_define_method_raw(mrb, c, a, m);
}
