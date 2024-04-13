mrb_top_run(mrb_state *mrb, struct RProc *proc, mrb_value self, unsigned int stack_keep)
{
  mrb_callinfo *ci;
  mrb_value v;

  if (!mrb->c->cibase) {
    return mrb_vm_run(mrb, proc, self, stack_keep);
  }
  if (mrb->c->ci == mrb->c->cibase) {
    return mrb_vm_run(mrb, proc, self, stack_keep);
  }
  ci = cipush(mrb);
  ci->mid = 0;
  ci->nregs = 1;   /* protect the receiver */
  ci->acc = CI_ACC_SKIP;
  ci->target_class = mrb->object_class;
  v = mrb_vm_run(mrb, proc, self, stack_keep);
  cipop(mrb);

  return v;
}
