mrb_class_superclass(mrb_state *mrb, mrb_value klass)
{
  struct RClass *c;

  c = mrb_class_ptr(klass);
  c = find_origin(c)->super;
  while (c && c->tt == MRB_TT_ICLASS) {
    c = find_origin(c)->super;
  }
  if (!c) return mrb_nil_value();
  return mrb_obj_value(c);
}
