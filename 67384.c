mrb_yield_cont(mrb_state *mrb, mrb_value b, mrb_value self, mrb_int argc, const mrb_value *argv)
{
  struct RProc *p;
  mrb_callinfo *ci;

  if (mrb_nil_p(b)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  if (mrb_type(b) != MRB_TT_PROC) {
    mrb_raise(mrb, E_TYPE_ERROR, "not a block");
  }

  p = mrb_proc_ptr(b);
  ci = mrb->c->ci;

  stack_extend(mrb, 3);
  mrb->c->stack[1] = mrb_ary_new_from_values(mrb, argc, argv);
  mrb->c->stack[2] = mrb_nil_value();
  ci->argc = -1;
  return mrb_exec_irep(mrb, self, p);
}
