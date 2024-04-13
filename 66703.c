fiber_check(mrb_state *mrb, mrb_value fib)
{
  struct RFiber *f = fiber_ptr(fib);

  mrb_assert(f->tt == MRB_TT_FIBER);
  if (!f->cxt) {
    mrb_raise(mrb, E_FIBER_ERROR, "uninitialized Fiber");
  }
  return f->cxt;
}
