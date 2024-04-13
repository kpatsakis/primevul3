mrb_obj_remove_instance_variable(mrb_state *mrb, mrb_value self)
{
  mrb_sym sym;
  mrb_value val;

  mrb_get_args(mrb, "n", &sym);
  mrb_iv_check(mrb, sym);
  val = mrb_iv_remove(mrb, self, sym);
  if (mrb_undef_p(val)) {
    mrb_name_error(mrb, sym, "instance variable %S not defined", mrb_sym2str(mrb, sym));
  }
  return val;
}
