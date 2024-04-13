mrb_obj_methods_m(mrb_state *mrb, mrb_value self)
{
  mrb_bool recur = TRUE;
  mrb_get_args(mrb, "|b", &recur);
  return mrb_obj_methods(mrb, recur, self, (mrb_method_flag_t)0); /* everything but private */
}
