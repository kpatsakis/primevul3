define_class(mrb_state *mrb, mrb_sym name, struct RClass *super, struct RClass *outer)
{
  struct RClass * c;

  if (mrb_const_defined_at(mrb, mrb_obj_value(outer), name)) {
    c = class_from_sym(mrb, outer, name);
    MRB_CLASS_ORIGIN(c);
    if (super && mrb_class_real(c->super) != super) {
      mrb_raisef(mrb, E_TYPE_ERROR, "superclass mismatch for Class %S (%S not %S)",
                 mrb_sym2str(mrb, name),
                 mrb_obj_value(c->super), mrb_obj_value(super));
    }
    return c;
  }

  c = mrb_class_new(mrb, super);
  setup_class(mrb, outer, c, name);

  return c;
}
