mrb_class_initialize(mrb_state *mrb, mrb_value c)
{
  mrb_value a, b;

  mrb_get_args(mrb, "|C&", &a, &b);
  if (!mrb_nil_p(b)) {
    mrb_yield_with_class(mrb, b, 1, &c, c, mrb_class_ptr(c));
  }
  return c;
}
