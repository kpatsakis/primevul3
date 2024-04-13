mrb_mod_cvar_set(mrb_state *mrb, mrb_value mod)
{
  mrb_value value;
  mrb_sym id;

  mrb_get_args(mrb, "no", &id, &value);
  check_cv_name_sym(mrb, id);
  mrb_cv_set(mrb, mod, id, value);
  return value;
}
