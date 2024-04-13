define_module(mrb_state *mrb, mrb_sym name, struct RClass *outer)
{
  struct RClass *m;

  if (mrb_const_defined_at(mrb, mrb_obj_value(outer), name)) {
    return module_from_sym(mrb, outer, name);
  }
  m = mrb_module_new(mrb);
  setup_class(mrb, outer, m, name);

  return m;
}
