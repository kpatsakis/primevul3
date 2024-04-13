mrb_class_defined_under(mrb_state *mrb, struct RClass *outer, const char *name)
{
  mrb_value sym = mrb_check_intern_cstr(mrb, name);
  if (mrb_nil_p(sym)) {
    return FALSE;
  }
  return mrb_const_defined_at(mrb, mrb_obj_value(outer), mrb_symbol(sym));
}
