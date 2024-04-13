mrb_fiber_yield(mrb_state *mrb, mrb_int len, const mrb_value *a)
{
  struct mrb_context *c = mrb->c;

  if (!c->prev) {
    mrb_raise(mrb, E_FIBER_ERROR, "can't yield from root fiber");
  }

  fiber_check_cfunc(mrb, c);
  c->prev->status = MRB_FIBER_RUNNING;
  c->status = MRB_FIBER_SUSPENDED;
  fiber_switch_context(mrb, c->prev);
  c->prev = NULL;
  if (c->vmexec) {
    c->vmexec = FALSE;
    mrb->c->ci->acc = CI_ACC_RESUMED;
  }
  MARK_CONTEXT_MODIFY(mrb->c);
  return fiber_result(mrb, a, len);
}
