mrb_define_method_raw(mrb_state *mrb, struct RClass *c, mrb_sym mid, mrb_method_t m)
{
  khash_t(mt) *h;
  khiter_t k;
  MRB_CLASS_ORIGIN(c);
  h = c->mt;

  if (MRB_FROZEN_P(c)) {
    if (c->tt == MRB_TT_MODULE)
      mrb_raise(mrb, E_FROZEN_ERROR, "can't modify frozen module");
    else
      mrb_raise(mrb, E_FROZEN_ERROR, "can't modify frozen class");
  }
  if (!h) h = c->mt = kh_init(mt, mrb);
  k = kh_put(mt, mrb, h, mid);
  kh_value(h, k) = m;
  if (MRB_METHOD_PROC_P(m) && !MRB_METHOD_UNDEF_P(m)) {
    struct RProc *p = MRB_METHOD_PROC(m);

    p->flags |= MRB_PROC_SCOPE;
    p->c = NULL;
    mrb_field_write_barrier(mrb, (struct RBasic*)c, (struct RBasic*)p);
    if (!MRB_PROC_ENV_P(p)) {
      MRB_PROC_SET_TARGET_CLASS(p, c);
    }
  }
  mc_clear_by_id(mrb, c, mid);
}
