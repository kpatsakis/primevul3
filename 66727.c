mrb_krn_class_defined(mrb_state *mrb, mrb_value self)
{
  mrb_value str;

  mrb_get_args(mrb, "S", &str);
  return mrb_bool_value(mrb_class_defined(mrb, RSTRING_PTR(str)));
}
