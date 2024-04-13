mrb_mod_const_missing(mrb_state *mrb, mrb_value mod)
{
  mrb_sym sym;

  mrb_get_args(mrb, "n", &sym);

  if (mrb_class_real(mrb_class_ptr(mod)) != mrb->object_class) {
    mrb_name_error(mrb, sym, "uninitialized constant %S::%S",
                   mod,
                   mrb_sym2str(mrb, sym));
  }
  else {
    mrb_name_error(mrb, sym, "uninitialized constant %S",
                   mrb_sym2str(mrb, sym));
  }
  /* not reached */
  return mrb_nil_value();
}
