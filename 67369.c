eval_under(mrb_state *mrb, mrb_value self, mrb_value blk, struct RClass *c)
{
  struct RProc *p;
  mrb_callinfo *ci;

  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  ci = mrb->c->ci;
  if (ci->acc == CI_ACC_DIRECT) {
    ci->target_class = c;
    return mrb_yield_cont(mrb, blk, self, 1, &self);
  }
  ci->target_class = c;
  p = mrb_proc_ptr(blk);
  ci->proc = p;
  ci->argc = 1;
  ci->mid = ci[-1].mid;
  if (MRB_PROC_CFUNC_P(p)) {
    stack_extend(mrb, 3);
    mrb->c->stack[0] = self;
    mrb->c->stack[1] = self;
    mrb->c->stack[2] = mrb_nil_value();
    return MRB_PROC_CFUNC(p)(mrb, self);
  }
  ci->nregs = p->body.irep->nregs;
  stack_extend(mrb, (ci->nregs < 3) ? 3 : ci->nregs);
  mrb->c->stack[0] = self;
  mrb->c->stack[1] = self;
  mrb->c->stack[2] = mrb_nil_value();
  ci = cipush(mrb);
  ci->nregs = 0;
  ci->target_class = 0;
  ci->pc = p->body.irep->iseq;
  ci->stackent = mrb->c->stack;
  ci->acc = 0;

  return self;
}
