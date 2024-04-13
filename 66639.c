mrb_define_method(mrb_state *mrb, struct RClass *c, const char *name, mrb_func_t func, mrb_aspec aspec)
{
  mrb_define_method_id(mrb, c, mrb_intern_cstr(mrb, name), func, aspec);
}
