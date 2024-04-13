mrb_mod_method_defined(mrb_state *mrb, mrb_value mod)
{
  mrb_sym id;

  mrb_get_args(mrb, "n", &id);
  return mrb_bool_value(mrb_obj_respond_to(mrb, mrb_class_ptr(mod), id));
}
