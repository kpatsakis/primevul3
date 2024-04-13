setup_class(mrb_state *mrb, struct RClass *outer, struct RClass *c, mrb_sym id)
{
  mrb_class_name_class(mrb, outer, c, id);
  mrb_obj_iv_set(mrb, (struct RObject*)outer, id, mrb_obj_value(c));
}
