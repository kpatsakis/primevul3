mrb_mod_cvar_defined(mrb_state *mrb, mrb_value mod)
{
  mrb_sym id;

  mrb_get_args(mrb, "n", &id);
  check_cv_name_sym(mrb, id);
  return mrb_bool_value(mrb_cv_defined(mrb, mod, id));
}
