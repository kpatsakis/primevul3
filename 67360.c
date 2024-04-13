mrb_module_get(mrb_state *mrb, const char *name)
{
  return mrb_class_get(mrb, name);
}
