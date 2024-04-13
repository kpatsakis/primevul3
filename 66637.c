mrb_define_class_method(mrb_state *mrb, struct RClass *c, const char *name, mrb_func_t func, mrb_aspec aspec)
{
  mrb_define_singleton_method(mrb, (struct RObject*)c, name, func, aspec);
}
