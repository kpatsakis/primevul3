mrb_mod_remove_cvar(mrb_state *mrb, mrb_value mod)
{
  mrb_value val;
  mrb_sym id;

  mrb_get_args(mrb, "n", &id);
  check_cv_name_sym(mrb, id);

  val = mrb_iv_remove(mrb, mod, id);
  if (!mrb_undef_p(val)) return val;

  if (mrb_cv_defined(mrb, mod, id)) {
    mrb_name_error(mrb, id, "cannot remove %S for %S",
                   mrb_sym2str(mrb, id), mod);
  }

  mrb_name_error(mrb, id, "class variable %S not defined for %S",
                 mrb_sym2str(mrb, id), mod);

 /* not reached */
 return mrb_nil_value();
}
