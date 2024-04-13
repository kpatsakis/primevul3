mrb_define_module(mrb_state *mrb, const char *name)
{
  return define_module(mrb, mrb_intern_cstr(mrb, name), mrb->object_class);
}
