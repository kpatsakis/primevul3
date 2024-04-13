mrb_undef_method(mrb_state *mrb, struct RClass *c, const char *name)
{
  undef_method(mrb, c, mrb_intern_cstr(mrb, name));
}
