mrb_define_module_id(mrb_state *mrb, mrb_sym name)
{
  return define_module(mrb, name, mrb->object_class);
}
