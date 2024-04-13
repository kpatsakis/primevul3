mrb_class_new_class(mrb_state *mrb, mrb_value cv)
{
  mrb_int n;
  mrb_value super, blk;
  mrb_value new_class;
  mrb_sym mid;

  n = mrb_get_args(mrb, "|C&", &super, &blk);
  if (n == 0) {
    super = mrb_obj_value(mrb->object_class);
  }
  new_class = mrb_obj_value(mrb_class_new(mrb, mrb_class_ptr(super)));
  mid = mrb_intern_lit(mrb, "initialize");
  if (!mrb_func_basic_p(mrb, new_class, mid, mrb_bob_init)) {
    mrb_funcall_with_block(mrb, new_class, mid, n, &super, blk);
  }
  mrb_class_inherited(mrb, mrb_class_ptr(super), mrb_class_ptr(new_class));
  return new_class;
}
