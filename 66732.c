mrb_obj_dup(mrb_state *mrb, mrb_value obj)
{
  struct RBasic *p;
  mrb_value dup;

  if (mrb_immediate_p(obj)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "can't dup %S", obj);
  }
  if (mrb_type(obj) == MRB_TT_SCLASS) {
    mrb_raise(mrb, E_TYPE_ERROR, "can't dup singleton class");
  }
  p = mrb_obj_alloc(mrb, mrb_type(obj), mrb_obj_class(mrb, obj));
  dup = mrb_obj_value(p);
  init_copy(mrb, dup, obj);

  return dup;
}
