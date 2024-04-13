mrb_equal_m(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;

  mrb_get_args(mrb, "o", &arg);
  return mrb_bool_value(mrb_equal(mrb, self, arg));
}
