mrb_class_inherited(mrb_state *mrb, struct RClass *super, struct RClass *klass)
{
  mrb_value s;
  mrb_sym mid;

  if (!super)
    super = mrb->object_class;
  super->flags |= MRB_FLAG_IS_INHERITED;
  s = mrb_obj_value(super);
  mc_clear_by_class(mrb, klass);
  mid = mrb_intern_lit(mrb, "inherited");
  if (!mrb_func_basic_p(mrb, s, mid, mrb_bob_init)) {
    mrb_value c = mrb_obj_value(klass);
    mrb_funcall_argv(mrb, s, mid, 1, &c);
  }
}
