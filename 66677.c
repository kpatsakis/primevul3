mrb_mod_remove_const(mrb_state *mrb, mrb_value mod)
{
  mrb_sym id;
  mrb_value val;

  mrb_get_args(mrb, "n", &id);
  check_const_name_sym(mrb, id);
  val = mrb_iv_remove(mrb, mod, id);
  if (mrb_undef_p(val)) {
    mrb_name_error(mrb, id, "constant %S not defined", mrb_sym2str(mrb, id));
  }
  return val;
}
