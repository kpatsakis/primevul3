mrb_mod_const_defined(mrb_state *mrb, mrb_value mod)
{
  mrb_sym id;
  mrb_bool inherit = TRUE;

  mrb_get_args(mrb, "n|b", &id, &inherit);
  check_const_name_sym(mrb, id);
  return const_defined(mrb, mod, id, inherit);
}
