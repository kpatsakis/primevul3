mrb_module_get(mrb_state *mrb, const char *name)
{
  return mrb_module_get_under(mrb, mrb->object_class, name);
}
