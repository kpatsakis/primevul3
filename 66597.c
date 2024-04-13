attr_writer(mrb_state *mrb, mrb_value obj)
{
  mrb_value name = mrb_proc_cfunc_env_get(mrb, 0);
  mrb_value val;

  mrb_get_args(mrb, "o", &val);
  mrb_iv_set(mrb, obj, to_sym(mrb, name), val);
  return val;
}
