mrb_mod_eqq(mrb_state *mrb, mrb_value mod)
{
  mrb_value obj;
  mrb_bool eqq;

  mrb_get_args(mrb, "o", &obj);
  eqq = mrb_obj_is_kind_of(mrb, obj, mrb_class_ptr(mod));

  return mrb_bool_value(eqq);
}
