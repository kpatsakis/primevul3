mrb_obj_class_m(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_value(mrb_obj_class(mrb, self));
}
