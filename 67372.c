mrb_exec_irep(mrb_state *mrb, mrb_value self, struct RProc *p)
{
  mrb_callinfo *ci = mrb->c->ci;
  int keep;

  mrb->c->stack[0] = self;
  ci->proc = p;
  if (MRB_PROC_CFUNC_P(p)) {
    return MRB_PROC_CFUNC(p)(mrb, self);
  }
  ci->nregs = p->body.irep->nregs;
  if (ci->argc < 0) keep = 3;
  else keep = ci->argc + 2;
  if (ci->nregs < keep) {
    stack_extend(mrb, keep);
  }
  else {
    stack_extend(mrb, ci->nregs);
    stack_clear(mrb->c->stack+keep, ci->nregs-keep);
  }

  ci = cipush(mrb);
  ci->nregs = 0;
  ci->target_class = 0;
  ci->pc = p->body.irep->iseq;
  ci->stackent = mrb->c->stack;
  ci->acc = 0;

  return self;
}
