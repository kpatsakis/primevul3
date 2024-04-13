fiber_yield(mrb_state *mrb, mrb_value self)
{
  mrb_value *a;
  mrb_int len;

  mrb_get_args(mrb, "*!", &a, &len);
  return mrb_fiber_yield(mrb, len, a);
}
