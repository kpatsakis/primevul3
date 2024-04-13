mrb_obj_ivar_get(mrb_state *mrb, mrb_value self)
{
  mrb_sym iv_name;

  mrb_get_args(mrb, "n", &iv_name);
  mrb_iv_check(mrb, iv_name);
  return mrb_iv_get(mrb, self, iv_name);
}
