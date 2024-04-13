mrb_obj_ceqq(mrb_state *mrb, mrb_value self)
{
  mrb_value v;
  mrb_int i, len;
  mrb_sym eqq = mrb_intern_lit(mrb, "===");
  mrb_value ary = mrb_ary_splat(mrb, self);

  mrb_get_args(mrb, "o", &v);
  len = RARRAY_LEN(ary);
  for (i=0; i<len; i++) {
    mrb_value c = mrb_funcall_argv(mrb, mrb_ary_entry(ary, i), eqq, 1, &v);
    if (mrb_test(c)) return mrb_true_value();
  }
  return mrb_false_value();
}
