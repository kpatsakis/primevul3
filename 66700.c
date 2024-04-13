to_str(mrb_state *mrb, mrb_value val)
{
  return check_type(mrb, val, MRB_TT_STRING, "String", "to_str");
}
