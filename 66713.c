mrb_fiber_alive_p(mrb_state *mrb, mrb_value self)
{
  struct mrb_context *c = fiber_check(mrb, self);
  return mrb_bool_value(c->status != MRB_FIBER_TERMINATED);
}
