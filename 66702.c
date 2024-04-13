top_define_method(mrb_state *mrb, mrb_value self)
{
  return mod_define_method(mrb, mrb_obj_value(mrb->object_class));
}
