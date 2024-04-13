check_const_name_str(mrb_state *mrb, mrb_value str)
{
  if (RSTRING_LEN(str) < 1 || !ISUPPER(*RSTRING_PTR(str))) {
    mrb_name_error(mrb, mrb_intern_str(mrb, str), "wrong constant name %S", str);
  }
}
