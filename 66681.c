mrb_mod_undef(mrb_state *mrb, mrb_value mod)
{
  struct RClass *c = mrb_class_ptr(mod);
  mrb_int argc;
  mrb_value *argv;

  mrb_get_args(mrb, "*", &argv, &argc);
  while (argc--) {
    undef_method(mrb, c, to_sym(mrb, *argv));
    argv++;
  }
  return mrb_nil_value();
}
