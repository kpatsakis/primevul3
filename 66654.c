mrb_instance_new(mrb_state *mrb, mrb_value cv)
{
  mrb_value obj, blk;
  mrb_value *argv;
  mrb_int argc;
  mrb_sym init;
  mrb_method_t m;

  mrb_get_args(mrb, "*&", &argv, &argc, &blk);
  obj = mrb_instance_alloc(mrb, cv);
  init = mrb_intern_lit(mrb, "initialize");
  m = mrb_method_search(mrb, mrb_class(mrb, obj), init);
  if (MRB_METHOD_CFUNC_P(m)) {
    mrb_func_t f = MRB_METHOD_CFUNC(m);
    if (f != mrb_bob_init) {
      f(mrb, obj);
    }
  }
  else {
    mrb_funcall_with_block(mrb, obj, init, argc, argv, blk);
  }

  return obj;
}
