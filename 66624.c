mrb_class_get(mrb_state *mrb, const char *name)
{
  return mrb_class_get_under(mrb, mrb->object_class, name);
}
