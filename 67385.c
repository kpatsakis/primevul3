mrb_yield_with_class(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv, mrb_value self, struct RClass *c)
{
  struct RProc *p;
  mrb_sym mid = mrb->c->ci->mid;
  mrb_callinfo *ci;
  int n = mrb->c->ci->nregs;
  mrb_value val;

  if (mrb_nil_p(b)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  if (mrb->c->ci - mrb->c->cibase > MRB_FUNCALL_DEPTH_MAX) {
    mrb_exc_raise(mrb, mrb_obj_value(mrb->stack_err));
  }
  p = mrb_proc_ptr(b);
  ci = cipush(mrb);
  ci->mid = mid;
  ci->proc = p;
  ci->stackent = mrb->c->stack;
  ci->argc = (int)argc;
  ci->target_class = c;
  ci->acc = CI_ACC_SKIP;
  mrb->c->stack = mrb->c->stack + n;
  ci->nregs = MRB_PROC_CFUNC_P(p) ? (int)(argc+2) : p->body.irep->nregs;
  stack_extend(mrb, ci->nregs);

  mrb->c->stack[0] = self;
  if (argc > 0) {
    stack_copy(mrb->c->stack+1, argv, argc);
  }
  mrb->c->stack[argc+1] = mrb_nil_value();

  if (MRB_PROC_CFUNC_P(p)) {
    val = MRB_PROC_CFUNC(p)(mrb, self);
    mrb->c->stack = mrb->c->ci->stackent;
    cipop(mrb);
  }
  else {
    val = mrb_run(mrb, p, self);
  }
  return val;
}
