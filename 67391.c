uvenv(mrb_state *mrb, int up)
{
  struct RProc *proc = mrb->c->ci->proc;
  struct REnv *e;

  while (up--) {
    proc = proc->upper;
    if (!proc) return NULL;
  }
  e = MRB_PROC_ENV(proc);
  if (e) return e;              /* proc has enclosed env */
  else {
    mrb_callinfo *ci = mrb->c->ci;
    mrb_callinfo *cb = mrb->c->cibase;

    while (cb <= ci) {
      if (ci->proc == proc) {
        return ci->env;
      }
      ci--;
    }
  }
  return NULL;
}
