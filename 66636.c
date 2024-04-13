mrb_define_class_id(mrb_state *mrb, mrb_sym name, struct RClass *super)
{
  if (!super) {
    mrb_warn(mrb, "no super class for '%S', Object assumed", mrb_sym2str(mrb, name));
  }
  return define_class(mrb, name, super, mrb->object_class);
}
