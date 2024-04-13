mod_define_method(mrb_state *mrb, mrb_value self)
{
  struct RClass *c = mrb_class_ptr(self);
  struct RProc *p;
  mrb_method_t m;
  mrb_sym mid;
  mrb_value proc = mrb_undef_value();
  mrb_value blk;

  mrb_get_args(mrb, "n|o&", &mid, &proc, &blk);
  switch (mrb_type(proc)) {
    case MRB_TT_PROC:
      blk = proc;
      break;
    case MRB_TT_UNDEF:
      /* ignored */
      break;
    default:
      mrb_raisef(mrb, E_TYPE_ERROR, "wrong argument type %S (expected Proc)", mrb_obj_value(mrb_obj_class(mrb, proc)));
      break;
  }
  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  p = (struct RProc*)mrb_obj_alloc(mrb, MRB_TT_PROC, mrb->proc_class);
  mrb_proc_copy(p, mrb_proc_ptr(blk));
  p->flags |= MRB_PROC_STRICT;
  MRB_METHOD_FROM_PROC(m, p);
  mrb_define_method_raw(mrb, c, mid, m);
  return mrb_symbol_value(mid);
}
