mrb_local_variables(mrb_state *mrb, mrb_value self)
{
  struct RProc *proc;
  mrb_irep *irep;
  mrb_value vars;
  size_t i;

  proc = mrb->c->ci[-1].proc;

  if (MRB_PROC_CFUNC_P(proc)) {
    return mrb_ary_new(mrb);
  }
  vars = mrb_hash_new(mrb);
  while (proc) {
    if (MRB_PROC_CFUNC_P(proc)) break;
    irep = proc->body.irep;
    if (!irep->lv) break;
    for (i = 0; i + 1 < irep->nlocals; ++i) {
      if (irep->lv[i].name) {
        mrb_hash_set(mrb, vars, mrb_symbol_value(irep->lv[i].name), mrb_true_value());
      }
    }
    if (!MRB_PROC_ENV_P(proc)) break;
    proc = proc->upper;
    if (!proc->c) break;
  }

  return mrb_hash_keys(mrb, vars);
}
