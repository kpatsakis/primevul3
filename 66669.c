mrb_mod_extend_object(mrb_state *mrb, mrb_value mod)
{
  mrb_value obj;

  mrb_check_type(mrb, mod, MRB_TT_MODULE);
  mrb_get_args(mrb, "o", &obj);
  mrb_include_module(mrb, mrb_class_ptr(mrb_singleton_class(mrb, obj)), mrb_class_ptr(mod));
  return mod;
}
