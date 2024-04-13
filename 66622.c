mrb_class_defined(mrb_state *mrb, const char *name)
{
  mrb_value sym = mrb_check_intern_cstr(mrb, name);
  if (mrb_nil_p(sym)) {
    return FALSE;
  }
  return mrb_const_defined(mrb, mrb_obj_value(mrb->object_class), mrb_symbol(sym));
}
