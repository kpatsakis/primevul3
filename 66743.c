mrb_obj_ivar_set(mrb_state *mrb, mrb_value self)
{
  mrb_sym iv_name;
  mrb_value val;

  mrb_get_args(mrb, "no", &iv_name, &val);
  mrb_iv_check(mrb, iv_name);
  mrb_iv_set(mrb, self, iv_name, val);
  return val;
}
