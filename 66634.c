mrb_const_get_sym(mrb_state *mrb, mrb_value mod, mrb_sym id)
{
  check_const_name_sym(mrb, id);
  return mrb_const_get(mrb, mod, id);
}
