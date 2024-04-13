mrb_include_module(mrb_state *mrb, struct RClass *c, struct RClass *m)
{
  int changed = include_module_at(mrb, c, find_origin(c), m, 1);
  if (changed < 0) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "cyclic include detected");
  }
}
