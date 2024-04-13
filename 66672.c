mrb_mod_initialize(mrb_state *mrb, mrb_value mod)
{
  mrb_value b;
  struct RClass *m = mrb_class_ptr(mod);
  boot_initmod(mrb, m); /* bootstrap a newly initialized module */
  mrb_get_args(mrb, "|&", &b);
  if (!mrb_nil_p(b)) {
    mrb_yield_with_class(mrb, b, 1, &mod, mod, m);
  }
  return mod;
}
