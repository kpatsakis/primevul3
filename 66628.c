mrb_class_name(mrb_state *mrb, struct RClass* c)
{
  mrb_value path = mrb_class_path(mrb, c);
  if (mrb_nil_p(path)) {
    path = mrb_str_new_lit(mrb, "#<Class:");
    mrb_str_concat(mrb, path, mrb_ptr_to_str(mrb, c));
    mrb_str_cat_lit(mrb, path, ">");
  }
  return RSTRING_PTR(path);
}
