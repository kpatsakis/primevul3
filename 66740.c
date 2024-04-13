mrb_obj_inspect(mrb_state *mrb, mrb_value obj)
{
  if ((mrb_type(obj) == MRB_TT_OBJECT) && mrb_obj_basic_to_s_p(mrb, obj)) {
    return mrb_obj_iv_inspect(mrb, mrb_obj_ptr(obj));
  }
  return mrb_any_to_s(mrb, obj);
}
