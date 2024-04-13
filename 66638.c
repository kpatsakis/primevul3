mrb_define_class_under(mrb_state *mrb, struct RClass *outer, const char *name, struct RClass *super)
{
  mrb_sym id = mrb_intern_cstr(mrb, name);
  struct RClass * c;

#if 0
  if (!super) {
    mrb_warn(mrb, "no super class for '%S::%S', Object assumed",
             mrb_obj_value(outer), mrb_sym2str(mrb, id));
  }
#endif
  c = define_class(mrb, id, super, outer);
  setup_class(mrb, outer, c, id);
  return c;
}
