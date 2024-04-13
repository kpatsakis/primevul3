mrb_prepend_module(mrb_state *mrb, struct RClass *c, struct RClass *m)
{
  struct RClass *origin;
  int changed = 0;

  if (!(c->flags & MRB_FLAG_IS_PREPENDED)) {
    origin = (struct RClass*)mrb_obj_alloc(mrb, MRB_TT_ICLASS, c);
    origin->flags |= MRB_FLAG_IS_ORIGIN | MRB_FLAG_IS_INHERITED;
    origin->super = c->super;
    c->super = origin;
    origin->mt = c->mt;
    c->mt = kh_init(mt, mrb);
    mrb_field_write_barrier(mrb, (struct RBasic*)c, (struct RBasic*)origin);
    c->flags |= MRB_FLAG_IS_PREPENDED;
  }
  changed = include_module_at(mrb, c, c, m, 0);
  if (changed < 0) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "cyclic prepend detected");
  }
}
