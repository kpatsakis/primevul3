include_class_new(mrb_state *mrb, struct RClass *m, struct RClass *super)
{
  struct RClass *ic = (struct RClass*)mrb_obj_alloc(mrb, MRB_TT_ICLASS, mrb->class_class);
  if (m->tt == MRB_TT_ICLASS) {
    m = m->c;
  }
  MRB_CLASS_ORIGIN(m);
  ic->iv = m->iv;
  ic->mt = m->mt;
  ic->super = super;
  if (m->tt == MRB_TT_ICLASS) {
    ic->c = m->c;
  }
  else {
    ic->c = m;
  }
  return ic;
}
