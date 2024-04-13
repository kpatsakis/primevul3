fiber_transfer(mrb_state *mrb, mrb_value self)
{
  struct mrb_context *c = fiber_check(mrb, self);
  mrb_value* a;
  mrb_int len;

  fiber_check_cfunc(mrb, mrb->c);
  mrb_get_args(mrb, "*!", &a, &len);

  if (c == mrb->root_c) {
    mrb->c->status = MRB_FIBER_TRANSFERRED;
    fiber_switch_context(mrb, c);
    MARK_CONTEXT_MODIFY(c);
    return fiber_result(mrb, a, len);
  }

  if (c == mrb->c) {
    return fiber_result(mrb, a, len);
  }

  return fiber_switch(mrb, self, len, a, FALSE, FALSE);
}
