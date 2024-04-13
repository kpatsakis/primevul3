fiber_eq(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);

  if (mrb_type(other) != MRB_TT_FIBER) {
    return mrb_false_value();
  }
  return mrb_bool_value(fiber_ptr(self) == fiber_ptr(other));
}
