mrb_mod_alias(mrb_state *mrb, mrb_value mod)
{
  struct RClass *c = mrb_class_ptr(mod);
  mrb_sym new_name, old_name;

  mrb_get_args(mrb, "nn", &new_name, &old_name);
  mrb_alias_method(mrb, c, new_name, old_name);
  return mrb_nil_value();
}
