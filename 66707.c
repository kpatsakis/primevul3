fiber_init(mrb_state *mrb, mrb_value self)
{
  static const struct mrb_context mrb_context_zero = { 0 };
  struct RFiber *f = fiber_ptr(self);
  struct mrb_context *c;
  struct RProc *p;
  mrb_callinfo *ci;
  mrb_value blk;
  size_t slen;

  mrb_get_args(mrb, "&", &blk);

  if (f->cxt) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot initialize twice");
  }
  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "tried to create Fiber object without a block");
  }
  p = mrb_proc_ptr(blk);
  if (MRB_PROC_CFUNC_P(p)) {
    mrb_raise(mrb, E_FIBER_ERROR, "tried to create Fiber from C defined method");
  }

  c = (struct mrb_context*)mrb_malloc(mrb, sizeof(struct mrb_context));
  *c = mrb_context_zero;
  f->cxt = c;

  /* initialize VM stack */
  slen = FIBER_STACK_INIT_SIZE;
  if (p->body.irep->nregs > slen) {
    slen += p->body.irep->nregs;
  }
  c->stbase = (mrb_value *)mrb_malloc(mrb, slen*sizeof(mrb_value));
  c->stend = c->stbase + slen;
  c->stack = c->stbase;

#ifdef MRB_NAN_BOXING
  {
    mrb_value *p = c->stbase;
    mrb_value *pend = c->stend;

    while (p < pend) {
      SET_NIL_VALUE(*p);
      p++;
    }
  }
#else
  memset(c->stbase, 0, slen * sizeof(mrb_value));
#endif

  /* copy receiver from a block */
  c->stack[0] = mrb->c->stack[0];

  /* initialize callinfo stack */
  c->cibase = (mrb_callinfo *)mrb_calloc(mrb, FIBER_CI_INIT_SIZE, sizeof(mrb_callinfo));
  c->ciend = c->cibase + FIBER_CI_INIT_SIZE;
  c->ci = c->cibase;
  c->ci->stackent = c->stack;

  /* adjust return callinfo */
  ci = c->ci;
  ci->target_class = MRB_PROC_TARGET_CLASS(p);
  ci->proc = p;
  mrb_field_write_barrier(mrb, (struct RBasic*)mrb_obj_ptr(self), (struct RBasic*)p);
  ci->pc = p->body.irep->iseq;
  ci->nregs = p->body.irep->nregs;
  ci[1] = ci[0];
  c->ci++;                      /* push dummy callinfo */

  c->fib = f;
  c->status = MRB_FIBER_CREATED;

  return self;
}
