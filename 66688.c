mrb_obj_new(mrb_state *mrb, struct RClass *c, mrb_int argc, const mrb_value *argv)
{
  mrb_value obj;
  mrb_sym mid;

  obj = mrb_instance_alloc(mrb, mrb_obj_value(c));
  mid = mrb_intern_lit(mrb, "initialize");
  if (!mrb_func_basic_p(mrb, obj, mid, mrb_bob_init)) {
    mrb_funcall_argv(mrb, obj, mid, argc, argv);
  }
  return obj;
}
