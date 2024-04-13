mrb_mod_include_p(mrb_state *mrb, mrb_value mod)
{
  mrb_value mod2;
  struct RClass *c = mrb_class_ptr(mod);

  mrb_get_args(mrb, "C", &mod2);
  mrb_check_type(mrb, mod2, MRB_TT_MODULE);

  while (c) {
    if (c->tt == MRB_TT_ICLASS) {
      if (c->c == mrb_class_ptr(mod2)) return mrb_true_value();
    }
    c = c->super;
  }
  return mrb_false_value();
}
