mrb_module_get_under(mrb_state *mrb, struct RClass *outer, const char *name)
{
  return module_from_sym(mrb, outer, mrb_intern_cstr(mrb, name));
}
