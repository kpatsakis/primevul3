check_cv_name_str(mrb_state *mrb, mrb_value str)
{
  const char *s = RSTRING_PTR(str);
  mrb_int len = RSTRING_LEN(str);

  if (len < 3 || !(s[0] == '@' && s[1] == '@')) {
    mrb_name_error(mrb, mrb_intern_str(mrb, str), "'%S' is not allowed as a class variable name", str);
  }
}
