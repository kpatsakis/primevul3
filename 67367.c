ecall(mrb_state *mrb)
{
  struct RProc *p;
  struct mrb_context *c = mrb->c;
  mrb_callinfo *ci = c->ci;
  struct RObject *exc;
  struct REnv *env;
  ptrdiff_t cioff;
  int ai = mrb_gc_arena_save(mrb);
  int i = --c->eidx;
  int nregs;

  if (i<0) return;
  if (ci - c->cibase > MRB_ECALL_DEPTH_MAX) {
    mrb_exc_raise(mrb, mrb_obj_value(mrb->stack_err));
  }
  p = c->ensure[i];
  if (!p) return;
  mrb_assert(!MRB_PROC_CFUNC_P(p));
  c->ensure[i] = NULL;
  nregs = p->upper->body.irep->nregs;
  if (ci->proc && !MRB_PROC_CFUNC_P(ci->proc) &&
      ci->proc->body.irep->nregs > nregs) {
    nregs = ci->proc->body.irep->nregs;
  }
  cioff = ci - c->cibase;
  ci = cipush(mrb);
  ci->stackent = mrb->c->stack;
  ci->mid = ci[-1].mid;
  ci->acc = CI_ACC_SKIP;
  ci->argc = 0;
  ci->proc = p;
  ci->nregs = p->body.irep->nregs;
  ci->target_class = MRB_PROC_TARGET_CLASS(p);
  env = MRB_PROC_ENV(p);
  mrb_assert(env);
  c->stack += nregs;
  exc = mrb->exc; mrb->exc = 0;
  if (exc) {
    mrb_gc_protect(mrb, mrb_obj_value(exc));
  }
  mrb_run(mrb, p, env->stack[0]);
  mrb->c = c;
  c->ci = c->cibase + cioff;
  if (!mrb->exc) mrb->exc = exc;
  mrb_gc_arena_restore(mrb, ai);
}
