obj_is_instance_of(mrb_state *mrb, mrb_value self)
{
  mrb_value arg;

  mrb_get_args(mrb, "C", &arg);

  return mrb_bool_value(mrb_obj_is_instance_of(mrb, self, mrb_class_ptr(arg)));
}
