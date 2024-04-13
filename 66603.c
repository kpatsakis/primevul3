check_if_class_or_module(mrb_state *mrb, mrb_value obj)
{
  if (!class_ptr_p(obj)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "%S is not a class/module", mrb_inspect(mrb, obj));
  }
}
