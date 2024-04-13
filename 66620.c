mrb_bob_not(mrb_state *mrb, mrb_value cv)
{
  return mrb_bool_value(!mrb_test(cv));
}
