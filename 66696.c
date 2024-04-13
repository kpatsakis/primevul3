prepare_singleton_class(mrb_state *mrb, struct RBasic *o)
{
  struct RClass *sc, *c;

  if (o->c->tt == MRB_TT_SCLASS) return;
  sc = (struct RClass*)mrb_obj_alloc(mrb, MRB_TT_SCLASS, mrb->class_class);
  sc->flags |= MRB_FLAG_IS_INHERITED;
  sc->mt = kh_init(mt, mrb);
  sc->iv = 0;
  if (o->tt == MRB_TT_CLASS) {
    c = (struct RClass*)o;
    if (!c->super) {
      sc->super = mrb->class_class;
    }
    else {
      sc->super = c->super->c;
    }
  }
  else if (o->tt == MRB_TT_SCLASS) {
    c = (struct RClass*)o;
    while (c->super->tt == MRB_TT_ICLASS)
      c = c->super;
    make_metaclass(mrb, c->super);
    sc->super = c->super->c;
  }
  else {
    sc->super = o->c;
    prepare_singleton_class(mrb, (struct RBasic*)sc);
  }
  o->c = sc;
  mrb_field_write_barrier(mrb, (struct RBasic*)o, (struct RBasic*)sc);
  mrb_field_write_barrier(mrb, (struct RBasic*)sc, (struct RBasic*)o);
  mrb_obj_iv_set(mrb, (struct RObject*)sc, mrb_intern_lit(mrb, "__attached__"), mrb_obj_value(o));
}
