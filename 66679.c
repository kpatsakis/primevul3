mrb_mod_remove_method(mrb_state *mrb, mrb_value mod)
{
  mrb_int argc;
  mrb_value *argv;

  mrb_get_args(mrb, "*", &argv, &argc);
  while (argc--) {
    remove_method(mrb, mod, to_sym(mrb, *argv));
    argv++;
  }
  return mod;
}
