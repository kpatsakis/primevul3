mod_define_singleton_method(mrb_state *mrb, mrb_value self)
{
  struct RProc *p;
  mrb_method_t m;
  mrb_sym mid;
  mrb_value blk = mrb_nil_value();

  mrb_get_args(mrb, "n&", &mid, &blk);
  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  p = (struct RProc*)mrb_obj_alloc(mrb, MRB_TT_PROC, mrb->proc_class);
  mrb_proc_copy(p, mrb_proc_ptr(blk));
  p->flags |= MRB_PROC_STRICT;
  MRB_METHOD_FROM_PROC(m, p);
  mrb_define_method_raw(mrb, mrb_class_ptr(mrb_singleton_class(mrb, self)), mid, m);
  return mrb_symbol_value(mid);
}
